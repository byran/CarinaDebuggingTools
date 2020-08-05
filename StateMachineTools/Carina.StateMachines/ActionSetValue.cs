using System.Xml.Serialization;

namespace Carina.StateMachines
{
    public class ActionSetValue : Action
    {
        // Todo : Add enum for this
        [XmlAttribute] public string Operator;

        public ValueInstance Variable;
        public ValueInstance Parameter;
    }
}