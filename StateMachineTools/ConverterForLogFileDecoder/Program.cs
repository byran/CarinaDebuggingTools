using System;
using System.Xml.Serialization;
using System.IO;
using System.Net.Mime;
using Carina.StateMachines;

namespace ConverterForLogFileDecoder
{
    class Program
    {
        static int Main(string[] args)
        {
            if (args.Length != 1)
            {
                Console.Error.WriteLine("Incorrect number of arguments");
                Console.Error.WriteLine("ConvertForLogFileDecoder <state machine file>");
                return 1;
            }
            using var file = new FileStream(args[0], FileMode.Open, FileAccess.Read);
            var serializer = new XmlSerializer(typeof(Project));

            var project = (Project) serializer.Deserialize(file);
            
            Console.WriteLine("#ifndef BUS_CONFIGURATION_INFORMATION_H");
            Console.WriteLine("#define BUS_CONFIGURATION_INFORMATION_H");
            Console.WriteLine();
            
            ModuleTypeEnums(project, Console.Out);
            Timeslots(project, Console.Out);
            
            Console.WriteLine("#endif");
            
            return 0;
        }

        static void ModuleTypeEnums(Project project, TextWriter writer)
        {
            writer.WriteLine("enum class ModuleType");
            writer.WriteLine("{");
            foreach (var module in project.ModuleTypes)
            {
                writer.WriteLine($"\t{module.ID},");
            }
            writer.WriteLine("\tUnspecified");
            writer.WriteLine("};");
            writer.WriteLine();
        }
        
        static void Timeslots(Project project, TextWriter writer)
        {
            writer.WriteLine("struct TimeSlots");
            writer.WriteLine("{");
            writer.WriteLine("\tModuleType type;");
            writer.WriteLine("\tuint32_t mask;");
            writer.WriteLine("};");
            writer.WriteLine();
            
            writer.WriteLine("namespace");
            writer.WriteLine("{");
            writer.WriteLine("\tTimeSlots timeSlots[]");
            writer.WriteLine("\t{");
            int slot = 0;
            foreach (var timeslot in project.TimeSlot)
            {
                Console.WriteLine("\t\t{ModuleType::" +
                                  (string.IsNullOrEmpty(timeslot.ModuleType) ? "Unspecified" : timeslot.ModuleType) +
                                  ", (1 << " + slot + ") },");
                ++slot;
            }
            writer.WriteLine("\t};");
            writer.WriteLine("};");
            writer.WriteLine();
        }
    }
}