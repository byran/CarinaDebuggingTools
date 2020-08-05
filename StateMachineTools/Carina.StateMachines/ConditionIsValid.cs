using System.Xml.Serialization;

namespace Carina.StateMachines
{
    public class ConditionIsValid : Condition
    {
        [XmlAttribute] public bool Valid;
        
        public ValueInstance Value;
    }
}