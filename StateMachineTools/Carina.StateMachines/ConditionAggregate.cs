using System.Xml.Serialization;

namespace Carina.StateMachines
{
    [XmlType("CConditionAggregate")]
    public class ConditionAggregate : Condition
    {
        // Todo : Add enum for this
        [XmlAttribute] public string Operator;
        public Condition ConditionLhs;
        public Condition ConditionRhs;
    }
}