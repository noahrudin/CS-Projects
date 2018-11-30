package fa.nfa;
import java.util.HashMap;
import java.util.LinkedHashSet;
import java.util.Set;

import fa.State;


/**
 * @author bryantbarreto, noahrudin
 *
 */
public class NFAState extends State {

	private HashMap<Character,LinkedHashSet<NFAState>> delta;//delta
	private boolean isFinal;//remembers its type
	
	/**
	 * Default constructor
	 * @param name the state name
	 */
	public NFAState(String name){
		initDefault(name);
		isFinal = false;
	}
	
	/**
	 * Overloaded constructor that sets the state type
	 * @param name the state name
	 * @param isFinal the type of state: true - final, false - nonfinal.
	 */
	public NFAState(String name, boolean isFinal){
		initDefault(name);
		this.isFinal = isFinal;
	}
	
	/**
	 * @param name of state 
	 * alternate constructor
	 */
	private void initDefault(String name){
		this.name = name;
		delta = new HashMap<Character, LinkedHashSet<NFAState>>();
	}
	
	/**
	 * Accessor for the state type
	 * @return true if final and false otherwise
	 */
	public boolean isFinal(){
		return isFinal;
	}
	

	/**
	 * Add the transition from this state object to another
	 * @param onSymb the alphabet symbol
	 * @param toState to DFA state
	 */
	public void addTransition(char onSymb, NFAState toState){
		if(!delta.containsKey(onSymb)) {
			LinkedHashSet<NFAState> stateSet = new LinkedHashSet<NFAState>();// creates new set of states if key has not been created yet
			stateSet.add(toState);
			delta.put(onSymb, stateSet);// adds new key and hash set to hashmap
		}else {
			delta.get(onSymb).add(toState); // adding another state to the state set inside of the hashmap
		}
	}
	
	/**
	 * Retrieves the state that this State transitions to
	 * on the given symbol
	 * @param symb - the alphabet symbol
	 * @return the new state 
	 */
	public LinkedHashSet<NFAState> getTo(char symb){
		return delta.get(symb);
	}
	
	
}