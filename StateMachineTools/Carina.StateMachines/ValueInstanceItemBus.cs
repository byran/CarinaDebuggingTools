using System.Xml.Serialization;

namespace Carina.StateMachines
{
    public class ValueInstanceItemBus : ValueInstance
    {
        [XmlAttribute] public string ValueItem;
        
        [XmlAttribute] public int Instance;
    }
}