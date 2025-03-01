#include <algorithm>                                                        // find(), shift_left(), shift_right(), equal(), swap(), lexicographical_compare()
#include <cmath>                                                            // min()
#include <compare>                                                          // weak_ordering
#include <cstddef>                                                          // size_t
#include <format>                                                           // format()
#include <initializer_list>                                                 // initializer_list
#include <iomanip>                                                          // setw()
#include <iostream>                                                         // istream, istream
#include <iterator>                                                         // distance(), next()
#include <source_location>                                                  // source_location
#include <stdexcept>                                                        // logic_error
#include <string>                                                           // string
#include <string_view>                                                      // string_view
#include <version>                                                          // defines feature-test macros, __cpp_lib_stacktrace

#if defined( __cpp_lib_stacktrace )                                         // Clang 19 does not yet support std::stacktrace.
  #include <stacktrace>                                                     // stacktrace
#endif


#include "GroceryItem.hpp"
#include "GroceryList.hpp"











///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Constructors and Destructors
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Initializer List Constructor
GroceryList::GroceryList( const std::initializer_list<GroceryItem> & initList )
{
  for( auto && groceryItem : initList )   insert( groceryItem, Position::BOTTOM );

  // Verify the internal grocery list state is still consistent amongst the four containers
  if( !containersAreConsistant() )   throw InvalidInternalState_Ex( "Container consistency error" );
}



// Exception Abstract Class Conversion Constructor
GroceryList::GroceryList_Ex::GroceryList_Ex( const std::string_view message, const std::source_location location )
  : std::logic_error( std::format( "{}\n detected in function \"{}\"\n at line {}\n in file \"{}\"\n\n********* Begin Stack Trace *********\n{}\n********* End Stack Trace *********\n",
                                    message,
                                    location.function_name(),
                                    location.line(),
                                    location.file_name(),
                                    #ifdef __cpp_lib_stacktrace
                                      std::stacktrace::current( 2 )                           // Let's not show exception object construction in the trace, so skip 2 (the base class and one derived class)
                                    #else
                                      "  Stack trace not available"
                                    #endif
                                 )
                    )
{}



// Exception Abstract Class Destructor must be defined, but the default behavior is just what is needed
GroceryList::GroceryList_Ex::~GroceryList_Ex() = default;












///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Queries
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// size() const
std::size_t GroceryList::size() const
{
  // Verify the internal grocery list state is still consistent amongst the four containers
  if( !containersAreConsistant() )   throw InvalidInternalState_Ex( "Container consistency error" );

  ///////////////////////// TO-DO (1) //////////////////////////////
return _gList_vector.size();
  /////////////////////// END-TO-DO (1) ////////////////////////////
}












///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Accessors
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// find() const
std::size_t GroceryList::find( const GroceryItem & groceryItem ) const
{
  // Verify the internal grocery list state is still consistent amongst the four containers
  if( !containersAreConsistant() )   throw InvalidInternalState_Ex( "Container consistency error" );

  ///////////////////////// TO-DO (2) //////////////////////////////
auto currentIterator = std::find(_gList_vector.begin(), _gList_vector.end(), groceryItem);

// difference_type => typically ptrdiff_t
auto index = currentIterator - _gList_vector.begin(); // no casts needed
return index; // if not found, index == size()
  /////////////////////// END-TO-DO (2) ////////////////////////////
}












///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Modifiers
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// insert( position )
void GroceryList::insert( const GroceryItem & groceryItem, Position position )
{
  // Convert the TOP and BOTTOM enumerations to an offset and delegate the work
  if     ( position == Position::TOP    )  insert( groceryItem, 0      );
  else if( position == Position::BOTTOM )  insert( groceryItem, size() );
  else                                     throw std::logic_error( "Unexpected insertion position" );         // Programmer error.  Should never hit this!
}



// insert( offset )
void GroceryList::insert( const GroceryItem & groceryItem, std::size_t offsetFromTop )        // insert provided grocery item at offsetFromTop, which places it before the current grocery item at offsetFromTop
{
  // Validate offset parameter before attempting the insertion.  std::size_t is an unsigned type, so no need to check for negative
  // offsets, and an offset equal to the size of the list says to insert at the end (bottom) of the list.  Anything greater than the
  // current size is an error.
  if( offsetFromTop > size() )   throw InvalidOffset_Ex( std::format( "Insertion position beyond end of current list size\n  position:     {:>2}\n  current size: {:>2}", offsetFromTop, size() ) );


  /**********  Prevent duplicate entries  ***********************/
  ///////////////////////// TO-DO (3) //////////////////////////////
  if( find( groceryItem ) != size() ) return;
  /////////////////////// END-TO-DO (3) ////////////////////////////


  // Inserting into the grocery list means you insert the grocery item into each of the containers (array, vector, list, and
  // forward_list). Because the data structure concept is different for each container, the way a grocery item gets inserted is a
  // little different for each.  You are to insert the grocery item into each container such that the ordering of all the containers
  // is the same.  A check is made at the end of this function to verify the contents of all four containers are indeed the same.


  { /**********  Part 1 - Insert into array  ***********************/
    ///////////////////////// TO-DO (4) //////////////////////////////
    if( _gList_array_size >= _gList_array.size() )
      throw CapacityExceeded_Ex(
        std::format( "Capacity Exceeded, array capacity: {}, list size: {}",
                     _gList_array.size(),
                     _gList_array_size ) );

    for( std::size_t i = _gList_array_size; i > offsetFromTop; --i )
    {
      _gList_array[i] = _gList_array[i - 1];
    }
    _gList_array[offsetFromTop] = groceryItem;
    ++_gList_array_size;
    /////////////////////// END-TO-DO (4) ////////////////////////////
  } // Part 1 - Insert into array




  { /**********  Part 2 - Insert into vector  **********************/
    ///////////////////////// TO-DO (5) //////////////////////////////
    _gList_vector.insert( std::next( _gList_vector.begin(), offsetFromTop ), groceryItem );
    /////////////////////// END-TO-DO (5) ////////////////////////////
  } // Part 2 - Insert into vector




  { /**********  Part 3 - Insert into doubly linked list  **********/
    ///////////////////////// TO-DO (6) //////////////////////////////
    _gList_dll.insert( std::next( _gList_dll.begin(), offsetFromTop ), groceryItem );
    /////////////////////// END-TO-DO (6) ////////////////////////////
  } // Part 3 - Insert into doubly linked list




  { /**********  Part 4 - Insert into singly linked list  **********/
    ///////////////////////// TO-DO (7) //////////////////////////////
    auto it = _gList_sll.before_begin();
    for( std::size_t i = 0; i < offsetFromTop; ++i ) ++it;
    _gList_sll.insert_after( it, groceryItem );
    /////////////////////// END-TO-DO (7) ////////////////////////////
  } // Part 4 - Insert into singly linked list


  // Verify the internal grocery list state is still consistent amongst the four containers
  if( !containersAreConsistant() )   throw InvalidInternalState_Ex( "Container consistency error" );
} // insert( const GroceryItem & groceryItem, std::size_t offsetFromTop )



// remove( groceryItem )
void GroceryList::remove( const GroceryItem & groceryItem )
{
  // Delegate to the version of remove() that takes an index as a parameter
  remove( find( groceryItem ) );
}



// remove( offset )
void GroceryList::remove( std::size_t offsetFromTop )
{
  // Removing from the grocery list means you remove the grocery item from each of the containers (array, vector, list, and
  // forward_list). Because the data structure concept is different for each container, the way a grocery item gets removed is a
  // little different for each.  You are to remove the grocery item from each container such that the ordering of all the containers
  // is the same.  A check is made at the end of this function to verify the contents of all four containers are indeed the same.

  if( offsetFromTop >= size() )   return;                                           // no change occurs if (zero-based) offsetFromTop >= size()


  { /**********  Part 1 - Remove from array  ***********************/
    ///////////////////////// TO-DO (8) //////////////////////////////
    for( std::size_t i = offsetFromTop; i < _gList_array_size - 1; ++i )
    {
      _gList_array[i] = _gList_array[i + 1];
    }
    --_gList_array_size;
    /////////////////////// END-TO-DO (8) ////////////////////////////
  } // Part 1 - Remove from array




  { /**********  Part 2 - Remove from vector  **********************/
    ///////////////////////// TO-DO (9) //////////////////////////////
   _gList_vector.erase( std::next( _gList_vector.begin(), offsetFromTop ) );
    /////////////////////// END-TO-DO (9) ////////////////////////////
  } // Part 2 - Remove from vector




  { /**********  Part 3 - Remove from doubly linked list  **********/
    ///////////////////////// TO-DO (10) //////////////////////////////
    _gList_dll.erase( std::next( _gList_dll.begin(), offsetFromTop ) );
    /////////////////////// END-TO-DO (10) ////////////////////////////
  } // Part 3 - Remove from doubly linked list




  {/**********  Part 4 - Remove from singly linked list  **********/
    ///////////////////////// TO-DO (11) //////////////////////////////
    auto it = _gList_sll.before_begin();
    for( std::size_t i = 0; i < offsetFromTop; ++i ) ++it;
    _gList_sll.erase_after( it );
    /////////////////////// END-TO-DO (11) ////////////////////////////
  } // Part 4 - Remove from singly linked list


  // Verify the internal grocery list state is still consistent amongst the four containers
  if( !containersAreConsistant() )   throw InvalidInternalState_Ex( "Container consistency error" );
} // remove( std::size_t offsetFromTop )



// moveToTop()
void GroceryList::moveToTop( const GroceryItem & groceryItem )
{
  ///////////////////////// TO-DO (12) //////////////////////////////
  auto pos = find( groceryItem );
  if( pos != size() )
  {
    remove( pos );
    insert( groceryItem, Position::TOP );
  }
  /////////////////////// END-TO-DO (12) ////////////////////////////
}



// operator+=( initializer_list )
GroceryList & GroceryList::operator+=( const std::initializer_list<GroceryItem> & rhs )
{
  ///////////////////////// TO-DO (13) //////////////////////////////
  for( auto && item : rhs )
  {
    insert( item, Position::BOTTOM );
  }
  /////////////////////// END-TO-DO (13) ////////////////////////////

  // Verify the internal grocery list state is still consistent amongst the four containers
  if( !containersAreConsistant() )   throw InvalidInternalState_Ex( "Container consistency error" );
  return *this;
}



// operator+=( GroceryList )
GroceryList & GroceryList::operator+=( const GroceryList & rhs )
{
  ///////////////////////// TO-DO (14) //////////////////////////////
  for( auto && item : rhs._gList_vector )
  {
    insert( item, Position::BOTTOM );
  }
  /////////////////////// END-TO-DO (14) ////////////////////////////

  // Verify the internal grocery list state is still consistent amongst the four containers
  if( !containersAreConsistant() )   throw InvalidInternalState_Ex( "Container consistency error" );
  return *this;
}












///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Relational Operators
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// operator<=>
std::weak_ordering GroceryList::operator<=>( GroceryList const & rhs ) const
{
  if( !containersAreConsistant() || !rhs.containersAreConsistant() )   throw InvalidInternalState_Ex( "Container consistency error" );

  ///////////////////////// TO-DO (15) //////////////////////////////
  auto lhsSize = size();
  auto rhsSize = rhs.size();
  auto commonSize = std::min( lhsSize, rhsSize );

  for( std::size_t i = 0; i < commonSize; ++i )
  {
    auto cmp = _gList_vector[i] <=> rhs._gList_vector[i];
    if( cmp != 0 ) return cmp;
  }
  return lhsSize <=> rhsSize;
  /////////////////////// END-TO-DO (15) ////////////////////////////
}



// operator==
bool GroceryList::operator==( GroceryList const & rhs ) const
{
  if( !containersAreConsistant() || !rhs.containersAreConsistant() )   throw InvalidInternalState_Ex( "Container consistency error" );

  ///////////////////////// TO-DO (16) //////////////////////////////
  if( size() != rhs.size() ) return false;
  for( std::size_t i = 0; i < size(); ++i )
  {
    if( _gList_vector[i] != rhs._gList_vector[i] ) return false;
  }
  return true;
  /////////////////////// END-TO-DO (16) ////////////////////////////
}












///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private member functions
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// containersAreConsistant() const
bool GroceryList::containersAreConsistant() const
{
  // Sizes of all containers must be equal to each other
  if(    _gList_array_size != _gList_vector.size()
      || _gList_array_size != _gList_dll.size()
      || _gList_array_size !=  gList_sll_size() ) return false;

  // Element content and order must be equal to each other
  auto current_array_position   = _gList_array .cbegin();
  auto current_vector_position  = _gList_vector.cbegin();
  auto current_dll_position     = _gList_dll   .cbegin();
  auto current_sll_position     = _gList_sll   .cbegin();

  auto end = _gList_vector.cend();
  while( current_vector_position != end )
  {
    if(    *current_array_position != *current_vector_position
        || *current_array_position != *current_dll_position
        || *current_array_position != *current_sll_position ) return false;

    // Advance the iterators to the next element in unison
    ++current_array_position;
    ++current_vector_position;
    ++current_dll_position;
    ++current_sll_position;
  }

  return true;
}



// gList_sll_size() const
std::size_t GroceryList::gList_sll_size() const
{
  ///////////////////////// TO-DO (17) //////////////////////////////
  return static_cast<std::size_t>( std::distance( _gList_sll.begin(), _gList_sll.end() ) );
  /////////////////////// END-TO-DO (17) ////////////////////////////
}












///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Non-member functions
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// operator<<
std::ostream & operator<<( std::ostream & stream, const GroceryList & groceryList )
{
  if( !groceryList.containersAreConsistant() )   throw GroceryList::InvalidInternalState_Ex( "Container consistency error" );

  // For each grocery item in the provided grocery list, insert the grocery item into the provided stream.  Each grocery item is
  // inserted on a new line and preceded with its index (aka offset from top)
  unsigned count = 0;
  for( auto && groceryItem : groceryList._gList_sll )   stream << '\n' << std::setw(5) << count++ << ":  " << groceryItem;

  return stream;
}



// operator>>
std::istream & operator>>( std::istream & stream, GroceryList & groceryList )
{
  if( !groceryList.containersAreConsistant() )   throw GroceryList::InvalidInternalState_Ex( "Container consistency error" );

  ///////////////////////// TO-DO (18) //////////////////////////////
  GroceryItem temp;
  while( stream >> temp )
  {
    groceryList.insert( temp, GroceryList::Position::BOTTOM );
  }
  /////////////////////// END-TO-DO (18) ////////////////////////////

  return stream;
}
