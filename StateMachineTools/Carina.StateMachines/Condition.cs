using System.Xml;
using System.Xml.Serialization;

namespace Carina.StateMachines
{
    [XmlInclude(typeof(ConditionComparison)),
     XmlInclude(typeof(ConditionAggregate)), 
     XmlInclude(typeof(ConditionIsValid)),
     XmlInclude(typeof(ConditionTimed))]
    public abstract class Condition
    {
        
    }
}