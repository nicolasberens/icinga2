/******************************************************************************
 * Icinga 2                                                                   *
 * Copyright (C) 2012 Icinga Development Team (http://www.icinga.org/)        *
 *                                                                            *
 * This program is free software; you can redistribute it and/or              *
 * modify it under the terms of the GNU General Public License                *
 * as published by the Free Software Foundation; either version 2             *
 * of the License, or (at your option) any later version.                     *
 *                                                                            *
 * This program is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU General Public License for more details.                               *
 *                                                                            *
 * You should have received a copy of the GNU General Public License          *
 * along with this program; if not, write to the Free Software Foundation     *
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.             *
 ******************************************************************************/

#ifndef TABLE_H
#define TABLE_H

namespace livestatus
{

class Filter;

/**
 * @ingroup livestatus
 */
class Table : public Object
{
public:
	typedef shared_ptr<Table> Ptr;
	typedef weak_ptr<Table> WeakPtr;

	typedef boost::function<void (const Object::Ptr&)> AddRowFunction;

	static Table::Ptr GetByName(const String& name);

	
	virtual String GetName(void) const = 0;

	vector<Object::Ptr> FilterRows(const shared_ptr<Filter>& filter);

	void AddColumn(const String& name, const Column& column);
	Column GetColumn(const String& name) const;
	vector<String> GetColumnNames(void) const;

protected:
	Table(void);

	virtual void FetchRows(const AddRowFunction& addRowFn) = 0;

	static Value ZeroAccessor(const Object::Ptr&);
	static Value OneAccessor(const Object::Ptr&);
	static Value EmptyStringAccessor(const Object::Ptr&);
	static Value EmptyArrayAccessor(const Object::Ptr&);
	static Value EmptyDictionaryAccessor(const Object::Ptr&);

private:
	map<String, Column> m_Columns;

	void FilteredAddRow(vector<Object::Ptr>& rs, const shared_ptr<Filter>& filter, const Object::Ptr& object);
};

}

#endif /* TABLE_H */
