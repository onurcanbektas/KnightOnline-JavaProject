package com.knightonline.shared.persistence.dao;

import java.io.Serializable;

import com.knightonline.shared.exception.DAOException;

/**
 * @author Mamaorha
 *
 */
public interface IGenericDAO <T, ID extends Serializable>
{
	public T getReference(ID key) throws DAOException;

	public T find(ID key) throws DAOException;

	public void persist(T entity) throws DAOException;

	public T merge(T entity) throws DAOException;

	public void remove(T entity) throws DAOException;
}
