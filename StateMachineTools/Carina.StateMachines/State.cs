using System.Collections.Generic;
using System.Diagnostics;
using System.Xml.Serialization;

namespace Carina.StateMachines
{
    [DebuggerDisplay("{ID} Transitions {Transitions.Count}, Entry {EntryActions.Count}, Exit {ExitActions.Count}")]
    public class State
    {
        [XmlAttribute] public string ID;

        public List<StateBase> StateBaseList;

        [XmlArray("Transitions")]
        public List<Transition> Transitions;

        public List<Action> EntryActions;
        public List<Action> ExitActions;
    }
}