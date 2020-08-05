using System.Xml.Serialization;

namespace Carina.StateMachines
{
    [XmlInclude(typeof(ValueInstanceItem)),
     XmlInclude(typeof(ValueInstanceItemBus))]
    public abstract class ValueInstance
    {
        
    }
}