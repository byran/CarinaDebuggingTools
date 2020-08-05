using System;
using System.Collections.Generic;
using System.Xml.Serialization;

namespace Carina.StateMachines
{
    public class ValueList
    {
        [XmlAttribute]
        public string ID;

        [XmlElement("ValueGroup")]
        public List<ValueGroup> ValueGroup;
    }
}