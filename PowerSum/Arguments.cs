using System;
using System.Collections.Specialized;
using System.Text;
using System.Text.RegularExpressions;

namespace Utility
{
    /// <summary>
    /// Arguments class
    /// </summary>
    public class ArgumentReader
    {
        // Variables
        private StringDictionary parameters = new StringDictionary();
        private string data = null;
        private static readonly Regex splitter = new Regex(@"^-{1,2}|^/|=|:", RegexOptions.IgnoreCase | RegexOptions.Compiled),
            remover = new Regex(@"^['""]?(.*?)['""]?$", RegexOptions.IgnoreCase | RegexOptions.Compiled);
        // Constructor
        public ArgumentReader(string[] args, out bool ParsingException)
        {
            ParsingException = false;
            string parameter = null;
            // Valid parameters forms:
            // {-,/,--}param{ ,=,:}((",')value(",'))
            // Examples: 
            // -param1 value1 --param2 /param3:"Test-:-work" 
            //   /param4=happy -param5 '--=nice=--'
            foreach (string arg in args)
            {
                // Look for new parameters (-,/ or --) and a
                // possible enclosed value (=,:)
                string[] parts = splitter.Split(arg, 3);
                switch (parts.Length)
                {
                    // Found a value (for the last parameter 
                    // found (space separator))
                    case 1:
                        if (parameter != null)
                        {
                            if (!parameters.ContainsKey(parameter))
                            {
                                parameters.Add(parameter, remover.Replace(parts[0], "$1"));
                            }
                            else
                            {
                                ParsingException = true;
                            }
                            parameter = null;
                        }
                        // else Error: no parameter waiting for a value (skipped)
                        else
                        {
                            if (data == null)
                            {
                                data = remover.Replace(parts[0], "$1");
                            }
                            else
                            {
                                ParsingException = true;
                            }
                        }
                        break;
                    // Found just a parameter
                    case 2:
                        // The last parameter is still waiting. 
                        // With no value, set it to true.
                        if (parameter != null)
                        {
                            if (!parameters.ContainsKey(parameter))
                            {
                                parameters.Add(parameter, string.Empty);
                            }
                            else
                            {
                                ParsingException = true;
                            }
                        }
                        parameter = parts[1];
                        break;

                    // Parameter with enclosed value
                    case 3:
                        // The last parameter is still waiting. 
                        // With no value, set it to true.
                        if (parameter != null)
                        {
                            if (!parameters.ContainsKey(parameter))
                            {
                                parameters.Add(parameter, string.Empty);
                            }
                            else
                            {
                                ParsingException = true;
                            }
                        }
                        parameter = parts[1];
                        // Remove possible enclosing characters (",')
                        if (!parameters.ContainsKey(parameter))
                        {
                            parameters.Add(parameter, remover.Replace(parts[2], "$1"));
                        }
                        parameter = null;
                        break;
                }
            }
            // In case a parameter is still waiting
            if (parameter != null)
            {
                if (!parameters.ContainsKey(parameter))
                {
                    parameters.Add(parameter, string.Empty);
                }
            }
        }
        public override string ToString()
        {
            StringBuilder str = new StringBuilder("{");
            foreach (System.Collections.DictionaryEntry pair in parameters)
            {
                str.Append(pair.Key);
                if (string.Empty.Equals(pair.Value))
                {
                    str.AppendFormat(" : {0}", pair.Value);
                }
                str.Append(", ");
            }
            return str.Append("} ").Append(data).ToString();
        }

        public String this[string param]
        {
            get
            {
                return parameters[param];
            }
        }
        public static implicit operator String(ArgumentReader self)
        {
            return self.data;
        }
    }
}