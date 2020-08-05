using System.Diagnostics;
using System.Xml.Serialization;

namespace Carina.StateMachines
{
    [DebuggerDisplay("{Variable} ({Instance})")]
    public class LanguageElementParameter
    {
        [XmlAttribute] public string Variable;
        [XmlAttribute] public int Instance;
    }
}