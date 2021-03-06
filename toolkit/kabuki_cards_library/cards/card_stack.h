/** Kabuki Arcade
    @file    ~/source/kabuki/arcade/cards/card_stack.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017-2018 Cale McCollough <calemccollough@gmail.com>;
             All right reserved (R). Licensed under the Apache License, Version
             2.0 (the "License"); you may not use this file except in
             compliance with the License. You may obtain a copy of the License
             [here](http://www.apache.org/licenses/LICENSE-2.0). Unless
             required by applicable law or agreed to in writing, software
             distributed under the License is distributed on an "AS IS" BASIS,
             WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
             implied. See the License for the specific language governing
             permissions and limitations under the License.
*/

#ifndef HEADER_FOR_KABUKI_ARCADE_CARDS_CARDSTACK
#define HEADER_FOR_KABUKI_ARCADE_CARDS_CARDSTACK

#include "card.h"
#include "deck.h"
#include "../../source/kabuki/data/array.h"

namespace kabuki { namespace cards {

/** Class that represents a stack of playing cards.
    A CardStack stores pointers to the Card objects in the Deck. When a card is
    removed from the stack, the pointer is deleted, and not the actual Card
    object in the Deck. You can extend the CardStack class to program the
    various elements of a card game use as a Hand and discard stack.
*/
class CardStack {
    public:

    /** Constructs an empty card stack with a max of 54 cards.
        @param size The number of array elements to reserve. */
    CardStack (int size = 54);
    
    /** Verbose constructor. */
    //CardStack (const CardStack& cards, int min_num_cards, 
    //           int max_num_cards = Deck::kFullDeckSize,
    //           bool is_visible = false);
    
    /** Creates a CardStack from the deck. */
    CardStack (Deck& deck, int min_cards = 0);
    
    /** Copy constructor deep copies the state of the other object.
        The copy constructor is use shuffle the CardStack. */
    CardStack (const CardStack& other);

    virtual ~CardStack ();

    /** Removes all the cards from the stack. */
    void Clear ();

    /** Compares this CardStack to the other CardStack.
        @return Returns 0 if the two have equal values, 1 if this stack is
                greater, and -1 if the other stack is greater. */
    virtual int Compare (CardStack& other);
        
    /** Returns the point value total of this stack of cards. */
    int GetValue ();

    /** Shuffles this CardStack. */
    void Shuffle ();

    /** Shuffles this CardStack. */
    void Shuffle (Deck& deck);

    /** Returns the size of this stack. */
    int GetSize () const;

    /** Returns the number of Card(s) in this stack. */
    int Length () const;

    /** Adds the card to top of the stack.
         @param card The Card to add.
         @return Returns the number of cards in the deck or -1 if the operation
                 failed. */
    int Push (Card* card);
    
    /** Inserts the card into the stack at the specified index.
        @return Returns 0 upon success
        @return Returns 1 if the index is out of bounds.
        @return Returns 2 if adding a card about exceed the manNumCards. */
    int InsertCard (Card* card, int index);
        
    /** Pushes the cards onto the stack.
        Functions takes the num_cards_take from the cards and adds
        them to this Stack.
        @pre    cards.getNuMCards () must be > num_cards_take.
        @pre    num_cards_take must be less than the maximum number of cards
                allowed.
        @return Returns 0 upon success.
        @return Returns -1 if the num_cards_take is < 0.
        @return Returns 1 if the num_cards_take is greater than the
                cards.getNumCards ()
        @return Returns 2 if the num_cards_take would put the user over the
                maxNumCards.
    */
    int Push (CardStack& cards);

    /** Pops a card off the stack. */
    Card* Draw ();
    
    /** Attempts to draw the given number of cards from the deck if there is
        enough. 
        @return Returns the number of cards drawn. */
    int DrawCards (CardStack& cards, int num_cards_take = 1);

    /** Counts the number of times this stack contains the given card. */
    int Count (Card* card);

    /** Returns true if this stack contains the given card. */
    int Contains (Card* card);

    /** Removes card from the stack.
        @return Returns true upon success and false if this stack doesn't 
                contain the pointer. */
    bool RemoveCard (Card* card);

    /** Copies the num_cards from the stack
        @pre    num_cards must be greater than the number of cards in the
                stock. */
    int SetCards (CardStack& stack, int num_cards);
        
    /** Draws the specified num_cards and adds them to this stack
        @pre    num_cards must be greater than the number of cards in the stock.
        @return Returns -1 if num*/
    int TakeCards (CardStack& stock, int num_cards);
        
    /** Returns a pointer to the Card at the specified index.
        @return Returns nil if the index is greater than the number of Cards
                in this Hand. */
    Card* GetCard (int index);
        
    /** Returns and removes the Card at the specified index. */
    Card* TakeCard (int index);
    
    /** Returns and removes the next Card off of the top of the Deck. */
    Card* TakeNextCard ();
    
    /** Returns and removes a random Card from the stack. */
    Card* TakeRandomCard ();

    /** Returns true if there are no more cards in the deck. */
    bool IsEmpty ();

    /** Prints this object to the log. */
    virtual _::Text& Print (_::Text& txt = _::Text ());

    /** Operator= makes this object = other object. */
    CardStack& operator= (const CardStack& other);

    /** Abstract Script Operation(s).
        @param index The index of the expression.
        @param expr  The Expression to read and write from.
        @return      Returns null upon success, a Set header upon query, and an 
                     error_t ticket upon Read-Write failure. */
    virtual const _::Operation* Star (uint index, _::Expression* expr);

    /** Handles Text input.
        @param text     Beginning of the Text buffer. 
        @param strand_end End of the Text buffer.
        @return Returns nil upon success and an error string upon failure. */
    virtual const char* Sudo (const char* text,
                            const char* strand_end);

    private:

    std::vector<Card*> cards_; //< Stack (Array) of Card pointers.

};      //< class CardStack
}       //< namespace cards
}       //< namespace kabuki
#endif  //< HEADER_FOR_KABUKI_ARCADE_CARDS_CARDSTACK
