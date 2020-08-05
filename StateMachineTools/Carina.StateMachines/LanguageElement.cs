using System.Collections.Generic;
using System.Diagnostics;
using System.Xml.Serialization;

namespace Carina.StateMachines
{
    [DebuggerDisplay("{ID}")]
    public class LanguageElement
    {
        [XmlAttribute] public string ID;

        [XmlArrayItem("ParameterValue")] public List<LanguageElementParameter> ParameterList;
        
        public List<LanguageText> LanguageTextList;
    }
}