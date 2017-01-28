package com.knightonline.shared.persistence.dao.impl;

import java.io.Serializable;
import java.lang.reflect.ParameterizedType;
import java.util.List;

import javax.persistence.EntityManager;
import javax.persistence.PersistenceContext;
import javax.persistence.PersistenceProperty;
import javax.persistence.Query;

import com.knightonline.shared.data.common.DynamicAttribute;
import com.knightonline.shared.data.constants.HibernateConstants;
import com.knightonline.shared.exception.DAOException;
import com.knightonline.shared.persistence.dao.IGenericDAO;

/**
 * @author Mamaorha
 *
 */
public abstract class AbstractHibernateDAO<T, ID extends Serializable> implements IGenericDAO<T, ID>
{
	private Class<T> entityBeanType;

	@PersistenceContext(unitName = HibernateConstants.PERSISTENCE_CONTEXT, properties = { @PersistenceProperty(name = HibernateConstants.SHOW_SQL, value = "true") })
	private EntityManager em;

	@SuppressWarnings("unchecked")
	public AbstractHibernateDAO()
	{
		Class<?> clazz = getClass();

		while (!(clazz.getGenericSuperclass() instanceof ParameterizedType))
		{
			clazz = clazz.getSuperclass();
		}

		this.entityBeanType = (Class<T>) ((ParameterizedType) clazz.getGenericSuperclass()).getActualTypeArguments()[0];
	}

	public EntityManager getEntityManager()
	{
		return this.em;
	}

	public void setEntityManager(EntityManager em)
	{
		this.em = em;
	}

	public Class<T> getEntityBeanType()
	{
		return this.entityBeanType;
	}

	@Override
	public T getReference(ID key) throws DAOException
	{
		try
		{
			return getEntityManager().getReference(this.entityBeanType, key);
		}

		catch (Exception e)
		{
			throw new DAOException(e);
		}
	}

	@Override
	public T find(ID key) throws DAOException
	{
		try
		{
			return getEntityManager().find(this.entityBeanType, key);
		}
		catch (Exception e)
		{
			throw new DAOException(e);
		}
	}

	@Override
	public void persist(T entity) throws DAOException
	{
		try
		{
			getEntityManager().persist(entity);
		}
		catch (Exception e)
		{
			throw new DAOException(e);
		}
	}

	@Override
	public T merge(T entity) throws DAOException
	{
		try
		{
			return getEntityManager().merge(entity);
		}
		catch (Exception e)
		{
			throw new DAOException(e);
		}
	}

	@Override
	public void remove(T entity) throws DAOException
	{
		try
		{
			getEntityManager().remove(entity);
		}
		catch (Exception e)
		{
			throw new DAOException(e);
		}
	}

	@SuppressWarnings("unchecked")
	private List<T> execute(Query query, boolean useCache, DynamicAttribute... attributes)
	{
		if (attributes != null)
		{
			for (DynamicAttribute dynamicAttribute : attributes)
			{
				query.setParameter(dynamicAttribute.getKey(), dynamicAttribute.getValue());
			}
		}

		query.setHint(HibernateConstants.CACHEABLE, useCache);

		return query.getResultList();
	}
	
	private T executeSingleResult(Query query, boolean useCache, DynamicAttribute... attributes)
	{
		List<T> list = execute(query, useCache, attributes);
		
		if(null != list && !list.isEmpty())
		{
			return list.get(0);
		}
		
		return null;
	}
	
	/**
	 * This method will execute an HQL query
	 */
	protected List<T> executeQuery(String query, DynamicAttribute... attributes)
	{
		Query q = getEntityManager().createQuery(query);
		return execute(q, Boolean.TRUE, attributes);
	}
	
	protected T executeQuerySingleResult(String query, DynamicAttribute... attributes)
	{
		Query q = getEntityManager().createQuery(query);
		return executeSingleResult(q, Boolean.TRUE, attributes);
	}
	
	/**
	 * This method will execute a Named HQL query and return the number of
	 * affected entities.
	 */
	protected List<T> executeNamedQuery(String namedQuery, boolean useCache, DynamicAttribute ... attributes)
	{
		Query q = getEntityManager().createNamedQuery(namedQuery);
		return execute(q, useCache, attributes);
	}
	
	protected T executeNamedQuerySingleResult(String namedQuery, boolean useCache, DynamicAttribute ... attributes)
	{
		Query q = getEntityManager().createNamedQuery(namedQuery);
		return executeSingleResult(q, useCache, attributes);
	}
	
	protected List<T> executeNativeQuery(String query, boolean useCache, DynamicAttribute ... attributes)
	{
		Query q = getEntityManager().createNativeQuery(query);
		return execute(q, useCache, attributes);
	}
	
	protected T executeNativeQuerySingleResult(String query, boolean useCache, DynamicAttribute ... attributes)
	{
		Query q = getEntityManager().createNativeQuery(query);
		return executeSingleResult(q, useCache, attributes);
	}
}
