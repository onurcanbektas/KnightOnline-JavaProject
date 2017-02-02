package com.knightonline.shared.network.packet;

import java.util.Map;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.atomic.AtomicBoolean;

import javax.annotation.PostConstruct;

import org.springframework.beans.factory.annotation.Autowired;

import com.knightonline.shared.network.packet.handlers.EchoHandler;

/**
 * @author Mamaorha
 *
 */
public abstract class PacketHandler implements Runnable
{
	private static final String PACKET_HANDLER_THREAD = "packet handler thread";
	private static final String UNHANDLED_PACKET_OPCODE = "unhandled packet - opcode [%s]";
	private static final String OPCODE_IS_ALREADY_HANDLED = "opcode [%s] is already handled";
	private static final String ECHOING_REQUEST = "Echoing request";
	
	@Autowired
	protected EchoHandler echoHandler;
	
	private Map<Short, IPacketHandler> packetTypesHandlers;
	protected BlockingQueue<Packet> requestQueue;
	protected ExecutorService threadExecutors;
	protected AtomicBoolean isAlive;

	@PostConstruct
	public void init()
	{
		this.packetTypesHandlers = new ConcurrentHashMap<>();
		this.requestQueue = new LinkedBlockingQueue<Packet>();

		this.isAlive = new AtomicBoolean(true);
		this.threadExecutors = Executors.newFixedThreadPool(getNumberOfThreads());
		this.threadExecutors.submit(this);

		initializePacketTypesHandlers();
	}

	protected abstract void initializePacketTypesHandlers();

	protected void registerHandler(short opcode, IPacketHandler handler)
	{
		if (null != packetTypesHandlers.get(opcode))
		{
			throw new InternalError(String.format(OPCODE_IS_ALREADY_HANDLED, opcode));
		}

		packetTypesHandlers.put(opcode, handler);
	}

	protected abstract int getNumberOfThreads();

	@Override
	public void run()
	{
		Thread.currentThread().setName(PACKET_HANDLER_THREAD);

		try
		{
			while (isAlive.get())
			{
				try
				{
					Packet requestPacket = requestQueue.take();

					IPacketHandler handler = packetTypesHandlers.get(requestPacket.getOpcode());

					if (null != handler)
					{
						handler.handlePacket(requestPacket);
					}

					else
					{
						System.out.println(String.format(UNHANDLED_PACKET_OPCODE, requestPacket.getOpcode()));
						System.out.println(ECHOING_REQUEST);
						
						echoHandler.handlePacket(requestPacket);
					}
				}
				
				catch (InterruptedException e)
				{
					isAlive.set(false);
					throw e;
				}
				
				catch (Throwable e)
				{
					e.printStackTrace();
				}
			}
		}

		catch (Exception e)
		{
			if (isAlive.get())
			{
				e.printStackTrace();
			}
		}

	}

	public void addWork(Packet packet) throws InterruptedException
	{
		requestQueue.put(packet);
	}
}
