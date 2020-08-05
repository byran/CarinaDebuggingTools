using System.Xml.Serialization;

namespace Carina.StateMachines
{
    [XmlInclude(typeof(ActionSetValue)),
     XmlInclude(typeof(ActionSwitchStateMachine)),
     XmlInclude(typeof(ActionSetScreen))]
    public abstract class Action
    {
        
    }
}