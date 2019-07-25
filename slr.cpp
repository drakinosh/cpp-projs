#include <iostream>
#include <vector>
#include <string>
#include <map>

//COMP409 mini-project
//Reference: The Dragon Book (2nd ed.)(Pearson LPE)
//thrown together in a few hours

struct ActionKey {
    int state_no;
    char grammar_sym;
}; 

bool operator<(const ActionKey& one, const ActionKey& other) {

    if (one.state_no == other.state_no) {
        return one.grammar_sym < other.grammar_sym;
    } else {
        return one.state_no < other.state_no;
    }
}

bool operator==(const ActionKey& one, const ActionKey& other) {
    return one.state_no == other.state_no && other.grammar_sym == other.grammar_sym;
}


enum {
    SHIFT=5, REDUCE, ACCEPT, REJECT
};

struct ActionValue {
    int action_type;
    int action_num;         //e.g. production no. for REDUCE, and state no. for SHIFT
    std::string action_string;
};

struct TerminalKey {
    int state_no;
    char non_terminal;

};

bool operator<(const TerminalKey& one, const TerminalKey& other)
{
    //return one.state_no < other.state_no && one.non_terminal < other.non_terminal;
    if (one.state_no == other.state_no) {
        return one.non_terminal < other.non_terminal;
    } else {
        return one.state_no < other.state_no;
    }
}

bool operator==(const TerminalKey& one, const TerminalKey& other)
{
    return one.state_no == other.state_no && other.non_terminal == other.non_terminal;
}

class Production;
bool isNonTerminal(char x);

std::vector<Production> productions;
char head;
std::string sub_head;
std::vector<char> grammar_symbols;
std::vector<char> non_terminals;

std::map<ActionKey,ActionValue> action_table;
std::map<TerminalKey, int> goto_table;
std::map<char,std::vector<char>> first_table;
std::map<char,std::vector<char>> follow_table;

class Production {
public:
    char head;
    std::string body;
    
    Production(char p_head, std::string p_body) {
        head = p_head;
        body = p_body;
    }

    bool operator==(const Production& other) {
        return head == other.head && body == other.body;
    }

};


class Item {
public:
        char head;
        int point_pos;
        std::string body;


        Item(char p_head, int p_point_pos, std::string p_body);
        Item(const Item& other);

        bool operator==(Item other) {
            return head == other.head && point_pos == other.point_pos && body == other.body;
        }
};

void printItem(Item t)
{
    std::cout << t.head << " -> ";
    const char *str = t.body.c_str();
    int i = t.point_pos;
    
    for (int j = 0; j < t.body.size(); j++) {
        
        if (j == i)
            std::cout << ".";
        std::cout << *(str+j);
    }

    std::cout << " [bodysize=" <<  t.body.size() <<  "] pos= " << t.point_pos << "\n";

}

Item::Item(char p_head, int p_point_pos, std::string p_body) 
{
   this->head = p_head;
   this->point_pos = p_point_pos;
   this->body = p_body;
}

Item::Item(const Item& other)
{
    head = other.head;
    point_pos = other.point_pos;
    body = other.body;
}

std::vector<Production> getProductionsFromHead(char head)
{
    std::vector<Production> productionss;
    for(auto x:productions){
        if (x.head == head){
             productionss.push_back(x);
        }
    }
    return productionss;
    
}

int getProductionIndex(Production prod)
{
    int i;

    for (i = 0; i < productions.size(); i++) {
        Production p = productions.at(i);
        
        if (p == prod) {
            return i;
        }
    }

    return -1;
}

bool itemInList(std::vector<Item> items, Item given_item)
{
    int i;

    for (i = 0; i < items.size(); i++) {
        Item item = items.at(i);

        if (given_item == item) {
            return true;
        }

    }
    return false;
}

void printItemSet(std::vector<Item> set)
{
    int i;

    for (i = 0; i < set.size(); i++) {
        Item it = set.at(i);

        std::cout << it.head << " -> " << it.body << " (pos) " << it.point_pos << std::endl;
    }
}

bool areItemSetsEquivalent(std::vector<Item> item_set_1, std::vector<Item> item_set_2)
{
    int i;
    
    if (item_set_1.size() != item_set_2.size()) {
        return false;
    }

    //set_1 is a subset of set_2
    for (i = 0; i < item_set_1.size(); i++) {
        Item each_item = item_set_1.at(i);
        //if even one item from set 1 is not in set 2, 
        if (!itemInList(item_set_2, each_item)) {
            /*
            std::cout<<"False for\n";
            std::cout << "set 1\n";
            printItemSet(item_set_1);
            std::cout << "set 2\n";
            printItemSet(item_set_2); */
            
            return false;
        }
    }
    //and set_2 is a subset of set_1
    for (i = 0; i < item_set_2.size(); i++) {
        Item each_item = item_set_2.at(i);
        if (!itemInList(item_set_1, each_item)) {
            return false;
        }
    }
    

    return true;
}



bool isItemSetInSuperSet(std::vector<std::vector<Item>> item_sets, std::vector<Item> given_item_set)
{
    int c;
    int i;

    for (c = 0; c < item_sets.size(); c++) {
        std::vector<Item> item_set = item_sets.at(c);
        
        if (areItemSetsEquivalent(item_set, given_item_set)) {
            return true;
        }
    }

    return false;

}

std::vector<Item> CLOSURE(std::vector<Item> I)
{

    std::vector<Item> closure_of_I;
    
    //initially, add every item in I to CLOSURE(I)
    int i, j;
    for (i = 0; i < I.size(); i++) {
        closure_of_I.push_back(I.at(i));
    } 

    //@ = alpha, &=beta
    //if A -> @.B& is in CLOSURE(I) and B -> [gamma] is a production,
    //  add the item B -> .[gamma] to CLOSURE(I) if it is not already there
    
    std::vector<Item> new_items;
    new_items.push_back(Item('X', 0, "ASDASFSAFASF"));
    
    //Apply until no more items can be added to the CLOSURE(I)
    //how to find point of exhaustion? 
    
    while (new_items.size() > 0 ) {
        new_items.erase(new_items.begin(), new_items.end());
        
        for (j = 0; j < closure_of_I.size(); j++) {
            Item item = closure_of_I.at(j);
            const char *body = item.body.c_str();
            char pChar = *(body + item.point_pos);

            if (isNonTerminal(pChar)) {

                std::vector<Production> prod_list = getProductionsFromHead(pChar);
                
                for (i = 0; i < prod_list.size(); i++) {
                    Production p = prod_list.at(i);
                    Item p_item = Item(p.head, 0, p.body); // A->[alpha]B[beta], B->[gamma], add B->.[gamma]
                    
                    if (!itemInList(closure_of_I, p_item)) {
                        new_items.push_back(p_item);
                    }
                }
            }
        }

        for (i = 0; i < new_items.size(); i++) {
            closure_of_I.push_back(new_items.at(i));
        }

    }

    return closure_of_I;
}

std::vector<Item> GOTO(std::vector<Item> items, char X) {
    int i;
    std::vector<Item> items_to_close; 

    if (X == 0 || X == 'e') {
        std::cout << "FATAL ERROR: GOTO got null\n";
    }
    
    //for each A->@.X&, add to items_to_close A->@X.&
    for (i = 0; i < items.size(); i++) {
        Item item = items.at(i);
        const char *i_body = item.body.c_str();
        char pointed_char = *(i_body + item.point_pos);

        if (pointed_char == X) {
            Item new_item = Item(item.head, item.point_pos + 1, item.body);
            //std::cout << "For GOTO, new_item\n";
            //printItem(new_item);
            items_to_close.push_back(new_item);
        }

    }

    //now, we need to find the closure of the set of items 'items_to_close'
    // note: it may be that items_to_close == items. I may have misunderstood the dragon book,
    //   but that would be a relatively simple mistake to fix.
    std::vector<Item> final_closure;
    final_closure = CLOSURE(items_to_close);
    

    return final_closure;

}

//final collection of items for the LR(0) automaton
std::vector<std::vector<Item>> getFinalSet()
{
    std::vector<std::vector<Item>> final_set;
    int i, j;
    //We start with the first production, S'->S (or, T->S for us)
    Production first_prod = productions.at(0);
    Item first_item = Item(first_prod.head, 0, first_prod.body);
    std::vector<Item> first_closure_set;
    first_closure_set.push_back(first_item);
    
    final_set.push_back(CLOSURE(first_closure_set));

    std::cout << "Got first item set\n";
   
    //repeat until no new [sets of items] are added on a round

    std::vector<std::vector<Item>> new_item_sets;
    std::vector<Item> trash;
    trash.push_back(Item('X', 0, "AASFASGSAXsgaXXAS"));
    new_item_sets.push_back(trash);
    
    while (new_item_sets.size() > 0) {
        
        new_item_sets.erase(new_item_sets.begin(), new_item_sets.end());

        for (i = 0; i < final_set.size(); i++) {

            std::vector<Item> item_set = final_set.at(i);
            //for each grammar symbol
            

            for (j = 0; j < grammar_symbols.size(); j++) {
                char gram_sym = grammar_symbols.at(j);

                std::vector<Item> goto_curr = GOTO(item_set, gram_sym);

                if (goto_curr.size() > 0 && !isItemSetInSuperSet(final_set, goto_curr)) {
                    new_item_sets.push_back(goto_curr); //note: do we need to add book_keeping info to goto_curr?
                }
            }
        }
        
        for (i = 0; i < new_item_sets.size(); i++) {
            std::cout << "Got new item set\n";
            final_set.push_back(new_item_sets.at(i));
        }
        
    }

    return final_set;
}

//becuase a nonterminal must be a production head
bool isNonTerminal(char X)
{
    return (X >= 65 && X <= 90);
}

bool isEpsilon(char X)
{
    return X == 'e';
}

int getItemSetIndex(std::vector<std::vector<Item>> mega_set, std::vector<Item> set)
{

    int i;

    for (i = 0; i < mega_set.size(); i++) {
        std::vector<Item> current_set = mega_set.at(i);

        if (areItemSetsEquivalent(current_set, set)) {
            return i;
        }
    }

    return -1; 
}

void buildParseTable(std::vector<std::vector<Item>> set_of_states)
{
    
    int curr_state = -1;

    for (int i = 0; i < set_of_states.size(); i++) {

        curr_state++;

        std::vector<Item> I_i = set_of_states.at(i);

        for (int j = 0; j < I_i.size(); j++) {

            
            Item curr_item = I_i.at(j);
            const char *i_body = curr_item.body.c_str();
            char gram_sym = *(i_body + curr_item.point_pos); 
            
            //if . to the right of a terminal, set ACTION[i,a]->"Reduce A -> [nonterm]" for all [nonterm]
            //   in FOLLOW(A)
            //i.e. body[pointer_pos] != '\0' and body[pointer_pos] != 'T'
            //
            
            //else if . is at the left of a terminal, set action to reduce, but first find the GOTO mapping
            
            if (!isNonTerminal(gram_sym) && gram_sym != 'e' && gram_sym != 0) { 

                std::cout << "Case (a) for " << gram_sym << "\n";
                std::vector<Item> curr_goto = GOTO(I_i, gram_sym);
                int index = getItemSetIndex(set_of_states, curr_goto);

                if (index == -1) {
                    std::cout << "ERROR: buildParseTable: [finding GOTO index] \n";
                    printItemSet(curr_goto);
                } else {
                    
                    ActionKey key;
                    key.state_no = curr_state;
                    key.grammar_sym = gram_sym;

                    ActionValue val;
                    val.action_type = SHIFT;
                    val.action_num = index; 
                    val.action_string = "SHIFT " + std::to_string(index);

                    std::cout << "added shift on " << gram_sym << " to state " << index << " for item " << curr_item.head << " -> " << curr_item.body<< " pos " << curr_item.point_pos << std::endl << "for I" << curr_state << "\n";
                    
                    action_table.insert(std::pair<ActionKey,ActionValue>(key,val));
                }

            } else if(gram_sym == 0 && curr_item.head != head) {
                std::cout << "Case (b)\n";
                    std::vector<char> follow_terminals = follow_table.at(curr_item.head);
                    int x;

                    for (x = 0; x < follow_terminals.size(); x++) {

                        char terminal = follow_terminals.at(x); //symbols, not terminals
                        
                        //find the relevant production and get its index
                        Production check_prod = Production(curr_item.head, curr_item.body);
                        int p_index = getProductionIndex(check_prod);

                        if (p_index == -1) {
                            std::cout << "ERROR: buildParseTable: [finding production index] \n";
                        } else {

                            ActionKey red_key;
                            red_key.state_no = curr_state;
                            red_key.grammar_sym = terminal;

                            ActionValue red_val;
                            red_val.action_type = REDUCE;
                            red_val.action_num = p_index;

                            red_val.action_string = "REDUCE " + std::string(1, curr_item.head) + " -> " + curr_item.body;


                            std::cout << "added reduce on " << terminal << " to term " << curr_item.head << " -> " << curr_item.body << " pos " << curr_item.point_pos << " for I" << curr_state << "\n";

                            action_table.insert(std::pair<ActionKey,ActionValue>(red_key, red_val));
                        }
                        
                    }
                

            } else if (curr_item.head == head && curr_item.point_pos == 1 && curr_item.body == sub_head) {
                std::cout << "Case (c) \n";
                ActionKey key_acc;
                key_acc.state_no = curr_state;
                key_acc.grammar_sym = '$';

                ActionValue val_acc;
                val_acc.action_type = ACCEPT;
                val_acc.action_string = "ACCEPT";

                action_table.insert(std::pair<ActionKey,ActionValue>(key_acc, val_acc));
                
            }
        }
        
        //set the GOTO table for each nonterminal
        for (int k = 0; k < non_terminals.size(); k++) {
            char nonterm = non_terminals.at(k);
            
            std::vector<Item> nonterm_goto = GOTO(I_i, nonterm);
            int g_index = getItemSetIndex(set_of_states, nonterm_goto);

            if (g_index == -1) {
                //std::cout << "ERROR: buildParseTable: goto for nonterminals\n";
                //std::cout << "error for GOTO(I" << i << ", " << nonterm << "), and set is: \n";
                //printItemSet(nonterm_goto);
            } else {
                TerminalKey tkey;
                tkey.state_no = curr_state;
                tkey.non_terminal = nonterm;

                int goto_ind = g_index;

                std::cout << "GOTO (" << curr_state << " , " << nonterm << " ) = " << g_index << "\n";

                goto_table.insert(std::pair<TerminalKey, int>(tkey, goto_ind));
            }
        }
    }


    

}


void parse(const char *input)
{
    //"stack" for state number
    std::vector<int> state_stack;
    std::vector<char> sym_stack;
    state_stack.push_back(0);
    char input_sym;
    int pos = 0;

    while (1) {
        int s = state_stack.back();
        std::cout << "New state: " << s << std::endl;
        //state_stack.pop_back(); 

        input_sym = *(input+pos);
        std::cout << "New input symbol: " << input_sym << std::endl;
        
        ActionKey mykey;
        //mykey.state_no = 64; //s
        //mykey.grammar_sym = 'z'; //input_sym
        mykey.state_no = s;
        mykey.grammar_sym = input_sym;
        
        ActionValue val = action_table.at(mykey);

        std::cout << "ACTION: " << val.action_string << std::endl;


        if (val.action_type == SHIFT) {
            int t = val.action_num;

            //std::cout << "Shift value: " << t << std::endl;
            state_stack.push_back(t);
            pos += 1;

        } else if (val.action_type == REDUCE) {
            int production_no = val.action_num;
            Production prod = productions.at(production_no);
            int len = prod.body.size(); //pop |[beta]| symbols(states) off the stack
            std::cout << "pop " << len << " items off stack\n";
            while (len > 0) {
                std::cout << "popping stack\n";
                state_stack.pop_back();
                len--;
            }

            int current_tos = state_stack.back(); //t
            std::cout << "current tos = " << current_tos << std::endl;
            std::cout << "prod head = " << prod.head;
            TerminalKey my_term_key;
            my_term_key.state_no = current_tos;
            my_term_key.non_terminal = prod.head;
            //push GOTO[t, A] onto the stack
            
            state_stack.push_back(goto_table.at(my_term_key));

            //PRINT THIS PRODUCTION. NOTE: overload << later
            std::cout << prod.head << " --> " << prod.body << std::endl;
        } else if (val.action_type == ACCEPT) {
            std::cout << "String accepted\n";
            break;
        } else {
            std::cout << "Parser error\n";
        }

        
    }


}


int main()
{
    productions.push_back(Production('T', "S")); //T stands for S'
    productions.push_back(Production('S', "qABC"));
    productions.push_back(Production('A', "a"));
    productions.push_back(Production('A', "bbD"));
    productions.push_back(Production('B', "a"));
    productions.push_back(Production('B', ""));
    productions.push_back(Production('C', "b"));
    productions.push_back(Production('C', ""));
    productions.push_back(Production('D', "C"));
    productions.push_back(Production('D', ""));
    
    head = 'T';
    sub_head = "S";
    grammar_symbols.push_back('T');
    grammar_symbols.push_back('S');
    grammar_symbols.push_back('A');
    grammar_symbols.push_back('B');
    grammar_symbols.push_back('C');
    grammar_symbols.push_back('D');
    grammar_symbols.push_back('a');
    grammar_symbols.push_back('b');
    grammar_symbols.push_back('c');
    //grammar_symbols.push_back('e'); //epsilon
    grammar_symbols.push_back('q');

    non_terminals.push_back('S');
    non_terminals.push_back('A');
    non_terminals.push_back('B');
    non_terminals.push_back('C');
    non_terminals.push_back('D');

    //first and follow
    
    std::vector<char> first_T, first_S, first_A, first_B, first_C, first_D;
    std::vector<char> follow_T, follow_S, follow_A, follow_B, follow_C, follow_D;

    first_T.push_back('q');
    first_S.push_back('q');
    first_A.push_back('a');
    first_A.push_back('b');
    first_B.push_back('a');
    first_B.push_back('e');
    first_C.push_back('b');
    first_C.push_back('e');
    first_D.push_back('b');
    first_D.push_back('e');

    first_table.insert(std::pair<char,std::vector<char>>('T', first_T));
    first_table.insert(std::pair<char,std::vector<char>>('S', first_S));
    first_table.insert(std::pair<char,std::vector<char>>('A', first_A));
    first_table.insert(std::pair<char,std::vector<char>>('B', first_B));
    first_table.insert(std::pair<char,std::vector<char>>('C', first_C));
    first_table.insert(std::pair<char,std::vector<char>>('D', first_D));

    
    //follow_T.push_back('$');
    follow_S.push_back('$');

    follow_C.push_back('$');
    follow_C.push_back('a');
    follow_C.push_back('b');

    follow_A.push_back('a');
    follow_A.push_back('$');
    follow_A.push_back('b');

    follow_D.push_back('a');
    follow_D.push_back('b');
    follow_D.push_back('$');

    follow_B.push_back('b');
    follow_B.push_back('$');

    follow_table.insert(std::pair<char,std::vector<char>>('T', follow_T));
    follow_table.insert(std::pair<char,std::vector<char>>('S', follow_S));
    follow_table.insert(std::pair<char,std::vector<char>>('C', follow_C));
    follow_table.insert(std::pair<char,std::vector<char>>('A', follow_A));
    follow_table.insert(std::pair<char,std::vector<char>>('D', follow_D));
    follow_table.insert(std::pair<char,std::vector<char>>('B', follow_B));

    std::vector<std::vector<Item>> set_of_states = getFinalSet();
    

    std::cout << "got final set\n";

    for (int i = 0; i < set_of_states.size(); i++) {
        std::vector<Item> iset = set_of_states.at(i);
        std::cout << "I" << i << " is: \n";
        printItemSet(iset);
    }

    buildParseTable(set_of_states);

    std::string input;
    std::cout << "Please input a string: \n";

    std::cin >> input;
    input += "$";

    const char *test_str = input.c_str();
    parse(test_str);
    //We identify the states by index. 
    //need to implement FOLLOW
    //Need to construct the parsing table

    //CHECK
    /*    
    std::vector<Item> check_items;
    std::vector<Item> init_items;
    Item it = Item('A', 2, "bbD");
    init_items.push_back(it);
    
    check_items = CLOSURE(init_items);

    int i;

    for (i = 0; i < check_items.size(); i++) {
        Item item = check_items.at(i);
        std::cout << item.head << " -> " << item.body;
        std::cout << std::endl;
    }
    */

    return 0;

}
