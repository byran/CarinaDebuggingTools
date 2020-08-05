using System.Xml.Serialization;

namespace Carina.StateMachines
{
    [XmlType("CConditionComparison")]
    public class ConditionComparison : Condition
    {
        // Todo : Add enum for this
        [XmlAttribute] public string Operator;

        public ValueInstance LhsValue;
        public ValueInstance RhsValue;
    }
}