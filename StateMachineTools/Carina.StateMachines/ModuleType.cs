using System.Collections.Generic;
using System.Diagnostics;
using System.Xml.Serialization;

namespace Carina.StateMachines
{
    [DebuggerDisplay("{ID}")]
    public class ModuleType
    {
        [XmlAttribute]
        public string ID;

        public List<ModuleVariant> ModuleVariants;
    }
}