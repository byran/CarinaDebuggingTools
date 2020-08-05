using System.Collections.Generic;
using System.Diagnostics;
using System.Xml.Serialization;

namespace Carina.StateMachines
{
    [DebuggerDisplay("Group {ID} ({ValueItem.Count})")]
    public class ValueGroup //: List<ValueItem>
    {
        [XmlAttribute]
        public string ID;
        
        [XmlElement("ValueItem")]
        public List<ValueItem> ValueItem;
    }
}