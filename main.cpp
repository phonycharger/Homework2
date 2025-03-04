#include <exception>
#include <iostream>
#include <sstream>                                                                    // istringstream
#include <typeinfo>

#include "GroceryItem.hpp"
#include "GroceryList.hpp"




namespace
{
  void basicScenario()
  {
    // Let's start a grocery list
    GroceryList thingsToBuy = { { "milk"     },
                                { "hot dogs" },
                                { "eggs"     },
                                { "bread"    } };

    // Changed my mind, I want to make sure I can get eggs before running out of money so I'm going to move that to the top of my list
    thingsToBuy.moveToTop( { "eggs" } );

    // Let's see what's on the list so far
    std::cout << "My Grocery List" << thingsToBuy << "\n\n";



    // Hmm ..., no.  Need to add a few more things
    thingsToBuy += { { "bananas" },
                     { "apples"  } };




    // My roommate also has a grocery list
    GroceryList roommatesList = { { "potato chips", "Frito Lays" },
                                  { "potato chips", "Ruffles"    },
                                  { "beer",         "Bud Lite"   },
                                  { "eggs"                       },
                                  { "pretzels"                   } };

    std::cout << "My roommate's Grocery List" << roommatesList << "\n\n";

    // Let's combine the lists (if different) and go shopping
    if( thingsToBuy != roommatesList ) thingsToBuy += roommatesList;


    // Oops, forgot butter.  Let's add that right before bread.
    thingsToBuy.insert( { "butter", "Lakes 'Ole" }, thingsToBuy.find( { "bread" } ) );

    // And beer, really?  You should be studying!
    thingsToBuy.remove( { "beer", "Bud Lite" } );


    // Now let's see what's on the list
    std::cout << "Combined Grocery Lists" << thingsToBuy << "\n\n";



    // Did you get it right?
    //
    // I could construct "expectedResults" with the list of grocery items as above, but I want to exercise GroceryList's extraction
    // operator.  To do that, I could:
    //   1) read the list of grocery items from standard input and type the data at the keyboard, but that is time consuming and
    //      error prone.  That's easily handled by creating a text file with grocery item data then redirecting that text file to
    //      standard input at the command line when launching the program.  But then I would have to separately maintain that file
    //      and include it in the set of student files.  More importantly, that would prevent you from doing that in the TO-DO
    //      section below.
    //   2) open a text file directly using either command line arguments that supply the file's path and name, or hard coding the
    //      file's path and name.  But that would require using (and understanding) the standard's ifstream interface, which is easy
    //      enough given sufficient time, but I don't want to go down that rabbit hole in this course.
    //   3) create an in-memory text file (well, more accurately, an in-memory input stream) and read from there.  This allows me to
    //      exercise the extraction operator while avoiding the perils described in options 1) and 2) above.
    // So, as a design decision, let's chose option 3), create an in-memory input stream and read from there.
    //
    //                                            UPC | Brand Name   | Product Name   | Price
    //                                            ----+--------------+----------------+--------
    std::istringstream expectedResultsStream( R"( "",   "",            "eggs",          0.0
                                                  "",   "",            "milk",          0.0
                                                  "",   "",            "hot dogs",      0.0
                                                  "",   "Lakes 'Ole",  "butter",        0.0
                                                  "",   "",            "bread",         0.0
                                                  "",   "",            "bananas",       0.0
                                                  "",   "",            "apples",        0.0
                                                  "",   "Frito Lays",  "potato chips",  0.0
                                                  "",   "Ruffles",     "potato chips",  0.0
                                                  "",   "",            "pretzels",      0.0 )"   // multi-line raw string literal of grocery items
                                               );

    GroceryList expectedResults;
    expectedResultsStream >> expectedResults;

    std::cout << "\nExpected results:" << expectedResults << "\n\n"
              << "\nActual results:  " << thingsToBuy     << "\n\n"
              << "\nTest results:    " << ( thingsToBuy != expectedResults ? "FAIL" : "PASS" ) << '\n';
  }
}




int main()
{
  try
  {
    basicScenario();


    ///////////////////////// TO-DO (1) //////////////////////////////
    {
      GroceryList partyList = {
        { "chicken wings", "Tyson" },
        { "soda",          "Coke"  },
        { "soda",          "Sprite"}
      };

      // Insert something at the top
      partyList.insert({ "snacks", "Doritos" }, GroceryList::Position::TOP);

      // Insert at the bottom
      partyList.insert({ "cake", "Entenmann's" }, GroceryList::Position::BOTTOM);

      // Remove a middle item (zero-based index)
      partyList.remove(2); // removing "soda", "Coke"

      // Move last item to top
      partyList.moveToTop({ "cake", "Entenmann's" });

      // We can declare secondList const if we never modify it
      const GroceryList secondList = {
        { "milk",    "Organic Valley" },
        { "cookies", "Oreo"           }
      };

      // Compare them
      if(partyList != secondList)
      {
        // Concatenate secondList into partyList
        partyList += secondList;
      }

      std::cout << "\nMy Party List:" << partyList << "\n\n";

      // Demo extraction via in-memory stream
      std::istringstream iss( R"( "12345", "Baskin Robbins", "Ice Cream", 6.99
                                   "",      "Lay's",          "Chips",     2.49 )" );
      iss >> partyList;
      std::cout << "After reading from stream:" << partyList << "\n\n";
    }
    /////////////////////// END-TO-DO (1) ////////////////////////////
  }

  catch( const std::exception & ex )
  {
    std::cerr << "Fatal Error:  unhanded exception \"" << typeid( ex ).name() << "\"\n"
              << ex.what() << '\n';
  }
}
