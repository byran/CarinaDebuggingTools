using System.Xml.Serialization;

namespace Carina.StateMachines
{
    public class ValueInstanceItem : ValueInstance
    {
        [XmlAttribute] public string ValueItem;
    }
}