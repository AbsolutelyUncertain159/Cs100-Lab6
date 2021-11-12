#include "gtest/gtest.h"
#include "spreadsheet.hpp"
#include "select.hpp"


TEST(SelectContainTest, SelectNotFound) 
{
  Spreadsheet* sheet = new Spreadsheet();
  sheet->set_column_names({ "First", "Last", "Age", "Major" });
  sheet->add_row({ "Daniel", "Karas", "50", "Poli Sci" });
  sheet->add_row({ "Ian", "Lopez", "22", "Bioengineering" });

  Select* Selector = new Select_Contains(sheet, "First", "l");
  EXPECT_FALSE(Selector->select(sheet, 1));

  delete Selector;
  delete sheet;
}

TEST(SelectContainTest, SelectFound)
{
	Spreadsheet* sheet = new Spreadsheet();
	sheet->set_column_names({ "First", "Last", "Age", "Major" });
	sheet->add_row({ "Daniel", "Karas", "50", "Poli Sci" });
	sheet->add_row({ "Ian", "Lopez", "22", "Bioengineering" });

	Select* Selector = new Select_Contains(sheet, "First", "D");
	EXPECT_TRUE(Selector->select(sheet, 0));

	delete Selector;
	delete sheet;
}

TEST(SelectContainTest, SelectFoundSubString)
{
	Spreadsheet* sheet = new Spreadsheet();
	sheet->set_column_names({ "First", "Last", "Age", "Major" });
	sheet->add_row({ "Daniel", "Karas", "50", "Poli Sci" });
	sheet->add_row({ "Daniela", "Lopez", "22", "Bioengineering" });

	Select* Selector = new Select_Contains(sheet, "First", "Daniel");
	EXPECT_TRUE(Selector->select(sheet, 1));

	delete Selector;
	delete sheet;
}

TEST(SelectNotTest, SelectNotFunction)
{
	Spreadsheet* sheet = new Spreadsheet();
	sheet->set_column_names({ "First", "Last", "Age", "Major" });
	sheet->add_row({ "Daniel", "Karas", "50", "Poli Sci" });
	sheet->add_row({ "Ian", "Lopez", "22", "Bioengineering" });

	Select* Selector = new Select_Not(new Select_Contains(sheet, "First", "B"));
	EXPECT_TRUE(Selector->select(sheet, 0));

	delete Selector;
	delete sheet;
}

TEST(SelectNotTest, SelectNotFunctionFalse)
{
	Spreadsheet* sheet = new Spreadsheet();
	sheet->set_column_names({ "First", "Last", "Age", "Major" });
	sheet->add_row({ "Daniel", "Karas", "50", "Poli Sci" });
	sheet->add_row({ "Ian", "Lopez", "22", "Bioengineering" });

	Select* Selector = new Select_Not(new Select_Contains(sheet, "First", "D"));
	EXPECT_FALSE(Selector->select(sheet, 0));

	delete Selector;
	delete sheet;
}
TEST(SelectOrTest, SelectOrFunction)
{
	Spreadsheet* sheet = new Spreadsheet();
	sheet->set_column_names({ "First", "Last", "Age", "Major" });
	sheet->add_row({ "Daniel", "Karas", "50", "Poli Sci" });
	sheet->add_row({ "Ian", "Lopez", "22", "Bioengineering" });

	Select* Selector = new Select_Or(new Select_Contains(sheet, "First", "D"), new Select_Contains(sheet, "Age", "50"));
	EXPECT_TRUE(Selector->select(sheet, 0));

	delete Selector;
	delete sheet;
}
TEST(SelectOrTest, SelectOrFunctionOneFound)
{
	Spreadsheet* sheet = new Spreadsheet();
	sheet->set_column_names({ "First", "Last", "Age", "Major" });
	sheet->add_row({ "Daniel", "Karas", "50", "Poli Sci" });
	sheet->add_row({ "Ian", "Lopez", "22", "Bioengineering" });

	Select* Selector = new Select_Or(new Select_Contains(sheet, "First", "D"), new Select_Contains(sheet, "Age", "22"));
	EXPECT_TRUE(Selector->select(sheet, 0));

	delete Selector;
	delete sheet;
}
TEST(SelectOrTest, SelectOrFunctionNotFound)
{
	Spreadsheet* sheet = new Spreadsheet();
	sheet->set_column_names({ "First", "Last", "Age", "Major" });
	sheet->add_row({ "Daniel", "Karas", "50", "Poli Sci" });
	sheet->add_row({ "Ian", "Lopez", "22", "Bioengineering" });

	Select* Selector = new Select_Or(new Select_Contains(sheet, "First", "x"), new Select_Contains(sheet, "Age", "22"));
	EXPECT_FALSE(Selector->select(sheet, 0));

	delete Selector;
	delete sheet;
}
TEST(SelectOrTest, SelectOrEmptyStringFound)
{
	Spreadsheet* sheet = new Spreadsheet();
	sheet->set_column_names({ "First", "Last", "Age", "Major" });
	sheet->add_row({ "Daniel", "Karas", "50", "Poli Sci" });
	sheet->add_row({ "Ian", "Lopez", "22", "Bioengineering" });
	sheet->add_row({ "Neftali", "Watkinson", "24", "Computer Science" });

	Select* Selector = new Select_Or(new Select_Contains(sheet, "Major", "q"), new Select_Contains(sheet, "Major", ""));
	ASSERT_TRUE(Selector->select(sheet, 0));
	delete Selector;
	delete sheet;
}

TEST(SelectAndTest, SelectAndFunction)
{
	Spreadsheet* sheet = new Spreadsheet();
	sheet->set_column_names({ "First", "Last", "Age", "Major" });
	sheet->add_row({ "Daniel", "Karas", "50", "Poli Sci" });
	sheet->add_row({ "Ian", "Lopez", "22", "Bioengineering" });

	Select* Selector = new Select_And(new Select_Contains(sheet, "First", "D"), new Select_Contains(sheet, "Last", "K"));
	EXPECT_TRUE(Selector->select(sheet, 0));

	delete Selector;
	delete sheet;
}

TEST(SelectAndTest, SelectAndFunctionOneNotFound)
{
	Spreadsheet* sheet = new Spreadsheet();
	sheet->set_column_names({ "First", "Last", "Age", "Major" });
	sheet->add_row({ "Daniel", "Karas", "50", "Poli Sci" });
	sheet->add_row({ "Ian", "Lopez", "22", "Bioengineering" });

	Select* Selector = new Select_And(new Select_Contains(sheet, "First", "z"), new Select_Contains(sheet, "Last", "K"));
	EXPECT_FALSE(Selector->select(sheet, 0));

	delete Selector;
	delete sheet;
}

TEST(SelectAndTest, SelectAndFunctionUpperCase)
{
	Spreadsheet* sheet = new Spreadsheet();
	sheet->set_column_names({ "First", "Last", "Age", "Major" });
	sheet->add_row({ "Daniel", "Karas", "50", "Poli Sci" });
	sheet->add_row({ "Ian", "Lopez", "22", "Bioengineering" });

	Select* Selector = new Select_And(new Select_Contains(sheet, "Last", "Lopez"), new Select_Contains(sheet, "Last", "lopez"));
	EXPECT_FALSE(Selector->select(sheet, 1));

	delete Selector;
	delete sheet;
}

TEST(SelectAndTest, SelectAndFunctionPluralCase)
{
	Spreadsheet* sheet = new Spreadsheet();
	sheet->set_column_names({ "First", "Last", "Age", "Major" });
	sheet->add_row({ "Daniel", "Karas", "50", "Poli Sci" });
	sheet->add_row({ "Ian", "Lopez", "22", "Bioengineering" });

	Select* Selector = new Select_And(new Select_Contains(sheet, "Major", "Poli Sci"), new Select_Contains(sheet, "Major", "Poli Scis"));
	EXPECT_FALSE(Selector->select(sheet, 1));

	delete Selector;
	delete sheet;
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
