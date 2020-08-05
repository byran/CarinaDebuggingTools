using System.Xml.Serialization;

namespace Carina.StateMachines
{
    public class StateBase
    {
        [XmlAttribute] public string Target;
    }
}