#ifndef __SELECT_HPP__
#define __SELECT_HPP__

#include <cstring>

class Select
{
public:
    virtual ~Select() = default;

    // Return true if the specified row should be selected.
    virtual bool select(const Spreadsheet* sheet, int row) const = 0;
};

// A common type of criterion for selection is to perform a comparison based on
// the contents of one column.  This class contains contains the logic needed
// for dealing with columns. Note that this class is also an abstract base
// class, derived from Select.  It introduces a new select function (taking just
// a string) and implements the original interface in terms of this.  Derived
// classes need only implement the new select function.  You may choose to
// derive from Select or Select_Column at your convenience.
class Select_Column: public Select
{
protected:
    int column;
public:
    Select_Column(const Spreadsheet* sheet, const std::string& name)
    {
        column = sheet->get_column_by_name(name);
    }

    virtual bool select(const Spreadsheet* sheet, int row) const
    {
        return select(sheet->cell_data(row, column));
    }

    // Derived classes can instead implement this simpler interface.
    virtual bool select(const std::string& s) const = 0;
};

class Select_Contains : public Select_Column
{
protected: 

    std::string StringToSearch;

    public:
        Select_Contains(const Spreadsheet* sheet, const std::string& name, const std::string& TargetString) : Select_Column(sheet, name)
        {
            StringToSearch = TargetString;
        }
        virtual bool select(const std::string& s) const
        {
            if (s.find(StringToSearch) != std::string::npos)
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }
};

class Select_Not : public Select
{
protected:

    Select* Selector;

public:
    Select_Not(Select* ParamSelect)
    {
        Selector = ParamSelect;
    }
     ~Select_Not()
    {
        delete Selector;
    }
    virtual bool select(const Spreadsheet* sheet, int row) const
    {
        return !(Selector->select(sheet, row));
        delete Selector;
    }

};

class Select_And : public Select
{
protected:

    Select* Selector1;
    Select* Selector2;

public:
    Select_And(Select* select1, Select* select2)
    {
        Selector1 = select1;
        Selector2 = select2;
    }
     ~Select_And()
    {
        delete Selector1;
        delete Selector2;
    }
    virtual bool select(const Spreadsheet* sheet, int row) const
    {
        return (Selector1->select(sheet, row) && Selector2->select(sheet, row));
        delete Selector1;
        delete Selector2;
    }
};

class Select_Or : public Select
{
protected:

    Select* Selector1;
    Select* Selector2;

public:
    Select_Or(Select* select1, Select* select2)
    {
        Selector1 = select1;
        Selector2 = select2;
    }
    ~Select_Or()
    {
        delete Selector1;
        delete Selector2;
    }
    virtual bool select(const Spreadsheet* sheet, int row) const
    {
        return (Selector1->select(sheet, row) || Selector2->select(sheet, row));
        delete Selector1;
        delete Selector2;
    }
};
#endif //__SELECT_HPP__
