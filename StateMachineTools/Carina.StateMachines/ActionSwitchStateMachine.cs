using System.Xml.Serialization;

namespace Carina.StateMachines
{
    public class ActionSwitchStateMachine : Action
    {
        [XmlAttribute] public string StateMachineInstance;
        
        [XmlAttribute] public string StateMachine;
    }
}