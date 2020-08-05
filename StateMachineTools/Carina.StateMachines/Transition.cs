using System;
using System.Collections.Generic;
using System.Xml.Serialization;

namespace Carina.StateMachines
{
    public class Transition
    {
        [XmlAttribute] public string Target;

        public Condition Condition;
    }
}