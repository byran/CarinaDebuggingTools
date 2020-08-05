using System.Collections.Generic;
using System.Diagnostics;
using System.Xml.Serialization;

namespace Carina.StateMachines
{
    [DebuggerDisplay("{ID}")]
    public class StateMachine
    {
        [XmlAttribute] public string ID;

        [XmlAttribute] public string StartState;

        [XmlElement("State")] public List<State> States;
        
        [XmlElement("LocalValueList")] public ValueList LocalValues;
    }
}