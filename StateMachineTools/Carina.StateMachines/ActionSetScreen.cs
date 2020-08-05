using System.Xml.Serialization;

namespace Carina.StateMachines
{
    public class ActionSetScreen : Action
    {
        [XmlAttribute] public string Screen;
    }
}