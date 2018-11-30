package fa.nfa;

import java.util.Collections;
import java.util.HashMap;
import java.util.LinkedHashSet;
import java.util.Set;
import fa.FAInterface;
import fa.State;
import fa.dfa.DFA;

/**
 * NFA class for implementation in NFADriver.java
 * 
 * @author noahrudin, bryantbarreto
 *
 */
public class NFA implements FAInterface, NFAInterface {
	// for original NFA Object
	private Set<NFAState> Q = new LinkedHashSet<NFAState>();
	private NFAState q0;

	// eclosure temp sets
	private LinkedHashSet<NFAState> eclosureTemp = new LinkedHashSet<NFAState>();
	private Set<Character> sigmaPrime = new LinkedHashSet<Character>();

	// keeps track of contents of DFA
	private HashMap<LinkedHashSet<NFAState>, String> dfaNameTracker = new HashMap<LinkedHashSet<NFAState>, String>();

	/**
	 * getDFA, returns a dfa
	 * 
	 * @return dfa, DFA for use in DFADriver.java
	 */
	@Override
	public DFA getDFA() {
		DFA dfa = new DFA();
		eclosureTemp = new LinkedHashSet<NFAState>();
		// make start set
		LinkedHashSet<NFAState> startSet = eClosure(getState(q0.getName()));
		LinkedHashSet<LinkedHashSet<NFAState>> stateSet = new LinkedHashSet<LinkedHashSet<NFAState>>();
		LinkedHashSet<LinkedHashSet<NFAState>> dfaTemp = new LinkedHashSet<LinkedHashSet<NFAState>>();
		// add start state to set
		stateSet.add(startSet);
		LinkedHashSet<NFAState> temp;
		boolean done = false;
		int stateSize = 0;
		while (!done) {// add states to state set
			done = true;
			for (char c : sigmaPrime) {// iterate over chars in alphabet
				for (Set<NFAState> states : stateSet) {
					temp = new LinkedHashSet<NFAState>();
					for (NFAState state : states) {// add states to tmp
						temp.addAll(getToState(state, c));
					}
					dfaTemp.add(temp);
				}
				stateSize = stateSet.size();
				stateSet.addAll(dfaTemp);
				if (stateSize < stateSet.size()) {
					done = false;
				}
			}
		}
		for (LinkedHashSet<NFAState> states : stateSet) {// determine start and final states
			if (isFinalSet(states)) {
				dfa.addFinalState(getName(states));
			} else {
				dfa.addState(getName(states));
			}
			dfa.addStartState(getName(startSet));
		}

		for (LinkedHashSet<NFAState> dfaState : stateSet) {// add transitions to states
			for (char c : sigmaPrime) {
				dfa.addTransition(getName(dfaState), c, getToStateName(dfaState, c));
			}
		}
		return dfa;
	}

	/**
	 * getToStateName, gets all transition states
	 * 
	 * @param dfaState,
	 *            from state
	 * @param c,
	 *            transition character
	 * @return getName(setOfToStates), the string associated with the state
	 */
	private String getToStateName(LinkedHashSet<NFAState> dfaState, char c) {
		LinkedHashSet<NFAState> setOfToStates = new LinkedHashSet<NFAState>();

		for (NFAState state : dfaState) {
			if (state.getTo(c) != null) {// check to see if the to state is null
				for (NFAState nfaState : state.getTo(c)) {
					if (!setOfToStates.contains(nfaState)) {// adding new state to set
						setOfToStates.add(nfaState);
						eclosureTemp = new LinkedHashSet<NFAState>();
						setOfToStates.addAll(eClosure(nfaState));
					}
				}
			}
		}
		return getName(setOfToStates);
	}

	/**
	 * getToState, gets all transition states
	 * 
	 * @param from,
	 *            the before state
	 * @param onSymb,
	 *            the transition character
	 * @return toStateTemp, the set of transition states
	 */
	@Override
	public LinkedHashSet<NFAState> getToState(NFAState from, char onSymb) {
		LinkedHashSet<NFAState> toStateTemp = new LinkedHashSet<NFAState>();
		if (from.getTo(onSymb) != null) {
			toStateTemp.addAll(from.getTo(onSymb));
			for (NFAState state : from.getTo(onSymb)) {// gather and add transition states
				eclosureTemp = new LinkedHashSet<NFAState>();
				toStateTemp.addAll(eClosure(state));
			}
			return toStateTemp;
		}
		return toStateTemp;
	}

	/**
	 * eClosure, represents epsilon closure on a given state
	 * 
	 * @param s,
	 *            the state from which to transition
	 * @return eclosureTemp, the set of states that can be transitioned to
	 */
	@Override
	public LinkedHashSet<NFAState> eClosure(NFAState s) {
		eclosureTemp.add(s);
		LinkedHashSet<NFAState> eTransitions = s.getTo('e'); // get all epsilon chars
		if (eTransitions != null) {
			for (NFAState n : eTransitions) {// add all e-transitions to the set
				if (!eclosureTemp.contains(n)) {
					eClosure(n);
				}
			}
		}
		return eclosureTemp;
	}

	/**
	 * addStartState, marks the given string as a start state
	 * 
	 * @param name,
	 *            the name of the state to be added
	 */
	@Override
	public void addStartState(String name) {
		NFAState s = getState(name);
		if (s == null) {// adds new state
			s = new NFAState(name);
			addState(name);
		}
		q0 = s; // sets q0 as the new state
	}

	/**
	 * addState, adds state to set
	 * 
	 * @param name,
	 *            name of state to be added
	 */
	@Override
	public void addState(String name) {
		Q.add(new NFAState(name));

	}

	/**
	 * addFinalState, creates state and marks it as final
	 * 
	 * @param name,
	 *            the name of the state to be added
	 */
	@Override
	public void addFinalState(String name) {
		Q.add(new NFAState(name, true));

	}

	/**
	 * addTransition, creates a transition between two states
	 * 
	 * @param fromState,
	 *            the state transitioned from
	 * @param onSymb,
	 *            the transition character
	 * @param toState,
	 *            the state to be transitioned to
	 */
	@Override
	public void addTransition(String fromState, char onSymb, String toState) {
		getState(fromState).addTransition(onSymb, getState(toState));
		if (!sigmaPrime.contains(onSymb) && onSymb != 'e') {// makes sure no e-closure
			sigmaPrime.add(onSymb);
		}
	}

	/**
	 * getStates, retrieves set of states
	 * 
	 * @return Q, the set of NFA states
	 */
	@Override
	public Set<NFAState> getStates() {
		return Q;
	}

	/**
	 * getFinalStates, retrieves set of final states
	 * 
	 * @return ret, the set of final states
	 */
	@Override
	public Set<NFAState> getFinalStates() {
		Set<NFAState> ret = new LinkedHashSet<NFAState>();
		for (NFAState e : Q) {// iterate over pool of states
			if (e.isFinal()) {// checks for final flag
				ret.add(e);
			}
		}
		return ret;
	}

	/**
	 * getStartState, retrieves the start state
	 * 
	 * @return q0, the start state
	 */
	@Override
	public State getStartState() {
		return q0;
	}

	/**
	 * getABC, gets the alphabet
	 * 
	 * @return sigmaPrime, the permitted chars
	 */
	@Override
	public Set<Character> getABC() {
		return sigmaPrime;
	}

	/**
	 * getState, retrieves a state
	 * 
	 * @param name,
	 *            the name of desired state
	 * @return e, the state with the given name
	 */
	public NFAState getState(String name) {
		for (NFAState e : Q) {
			if (e.getName().equals(name)) {// checks for state in state set
				return e;
			}
		}
		return null;
	}

	/**
	 * getName, creates a unique name for states
	 * 
	 * @param e,
	 *            set of states
	 * @return uniqueName, the name given in the algorithm
	 */
	private String getName(LinkedHashSet<NFAState> e) {
		String uniqueName = "";

		if (!dfaNameTracker.containsKey(e)) {// returns new name if not in HashMap
			for (NFAState state : e) {
				uniqueName += state.getName();
			}
			dfaNameTracker.put(e, uniqueName);
			return uniqueName;
		}

		return dfaNameTracker.get(e); // returns name already present
	}

	/**
	 * isFinalSet, checks if the set contains a final flag
	 * 
	 * @param e,
	 *            set of states
	 * @return true/false, whether the set contains a flag marked as final
	 */
	private boolean isFinalSet(LinkedHashSet<NFAState> e) {
		for (NFAState state : e) {//iterate over the set
			if (state.isFinal()) {
				return true;
			}
		}
		return false;
	}

}