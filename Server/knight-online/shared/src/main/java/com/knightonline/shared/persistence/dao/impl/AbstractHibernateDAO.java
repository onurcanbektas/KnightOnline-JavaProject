package com.knightonline.shared.persistence.dao.impl;

import java.io.Serializable;
import java.lang.reflect.ParameterizedType;
import java.util.List;

import javax.persistence.EntityManager;
import javax.persistence.PersistenceContext;
import javax.persistence.PersistenceProperty;
import javax.persistence.Query;

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

	/**
	 * This method will execute an HQL query
	 */
	protected List<?> executeQuery(String query, String namedParams[], Object params[])
	{
		Query q = getEntityManager().createQuery(query);

		if (namedParams != null)
		{
			for (int i = 0; i < namedParams.length; i++)
			{
				q.setParameter(namedParams[i], params[i]);
			}
		}

		q.setHint(HibernateConstants.CACHEABLE, Boolean.TRUE);

		return q.getResultList();
	}

	@SuppressWarnings({ "rawtypes" })
	protected List executeQuery(String query)
	{
		return executeQuery(query, null, null);
	}

	/**
	 * This method will execute a Named HQL query and return the number of
	 * affected entities.
	 */
	protected List<?> executeNamedQuery(String namedQuery, String namedParams[], Object params[], boolean useCache)
	{
		Query q = getEntityManager().createNamedQuery(namedQuery);

		if (namedParams != null)
		{
			for (int i = 0; i < namedParams.length; i++)
			{
				q.setParameter(namedParams[i], params[i]);
			}
		}

		q.setHint(HibernateConstants.CACHEABLE, new Boolean(useCache));

		return q.getResultList();
	}
	
	protected List<?> executeNamedQueryLimited(String namedQuery, String namedParams[], Object params[], boolean useCache, int maxRows)
	{
		Query q = getEntityManager().createNamedQuery(namedQuery);
		q.setMaxResults(maxRows);

		if (namedParams != null)
		{
			for (int i = 0; i < namedParams.length; i++)
			{
				q.setParameter(namedParams[i], params[i]);
			}
		}

		q.setHint(HibernateConstants.CACHEABLE, new Boolean(useCache));

		return q.getResultList();
	}

	protected List<?> executeNamedQuery(String namedQuery, boolean useCache)
	{
		return executeNamedQuery(namedQuery, null, null, useCache);
	}

	protected List<?> executeNativeQuery(String query, String namedParams[], Object params[], boolean useCache)
	{
		Query q = getEntityManager().createNativeQuery(query);

		if (namedParams != null)
		{
			for (int i = 0; i < namedParams.length; i++)
			{
				q.setParameter(i + 1, String.valueOf(params[i]));
			}
		}

		q.setHint(HibernateConstants.CACHEABLE, new Boolean(useCache));
		return q.getResultList();
	}

	protected List<?> executeNativeQuery(String query, boolean useCache)
	{
		return executeNativeQuery(query, null, null, useCache);
	}
}