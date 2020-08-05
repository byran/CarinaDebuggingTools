using System.Xml.Serialization;

namespace Carina.StateMachines
{
    [XmlType("CConditionTimed")]
    public class ConditionTimed : Condition
    {
        public int Delay;
    }
}