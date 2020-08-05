using System;
using System.Diagnostics;
using System.Xml.Serialization;
using System.IO;
using System.Linq;

using Carina.StateMachines;

namespace ConvertProject
{
    [DebuggerDisplay("Global.{gid}.{vid}")]
    struct ValueID
    {
        public string gid;
        public string vid;
        
    }
    
    class Program
    {
        static void Main(string[] args)
        {
            var path = "/home/byran/Projects/work/CarinaCompressedProject/StateMachines.xml";
            using var file = new FileStream(path, FileMode.Open, FileAccess.Read);
            var serializer = new XmlSerializer(typeof(Project));

            var project = (Project) serializer.Deserialize(file);

            var typeR = project.ModuleTypes[0].ModuleVariants[0];

/*            var variables = (from groups in typeR.GlobalValues.ValueGroup
                from variable in groups.ValueItem
                orderby variable.Index
                select new ValueID() {
                gid = groups.ID, vid = variable.ID
            }).ToList();

            var states = (from stateMachine in typeR.StateMachineList
                    where stateMachine.LocalValues.ValueGroup.Count > 0
                    select stateMachine
                ).ToList();
*/            
            Console.WriteLine(typeR.GlobalID["GlobalValues.Bus.Map"]);

            Console.WriteLine("Bus value count: " + project.BusValuesIndexed.Count);
            foreach (var type in project.ModuleTypes)
            {
                foreach (var variant in type.ModuleVariants)
                {
                    Console.WriteLine(variant.ID + ": " + variant.GlobalID.Count);
                }
            }
        }
    }
}