using System;
using Math;
using Utility;

namespace PowerSum
{
    class Program
    {
        [System.Runtime.InteropServices.DllImport("Kernel32.dll", SetLastError = true)]
        extern static uint GetConsoleProcessIds(uint[] ids, uint len);

        static bool inSuiGenerisConsole()
        {
            var retval = GetConsoleProcessIds(new uint[] { 0 }, 1);
            if (0==retval)
                System.Runtime.InteropServices.Marshal.ThrowExceptionForHR(System.Runtime.InteropServices.Marshal.GetHRForLastWin32Error());
            return 1 == retval;
        }

        static void Main(string[] _in)
        {
            ArgumentReader args = new ArgumentReader(_in, out _);
            bool pretty = (
                args["pretty"] == "" || 
                "on".Equals(args["pretty"], StringComparison.CurrentCultureIgnoreCase) || 
                (!"off".Equals(args["pretty"], StringComparison.CurrentCultureIgnoreCase) && 
                    (string.IsNullOrEmpty(args["p"]) || 
                        (string.IsNullOrEmpty(args["x"]) && (string)args == null)
                    )
                )
            );
            ulong power;
            if (string.IsNullOrEmpty(args["p"]))
            {
                Console.Write("What power should PowerSum sum?");
                power = ulong.Parse(Console.ReadLine());
            }
            else
            {
                power = ulong.Parse(args["p"]);
            }
            if (pretty)
            {
                Console.WriteLine("Working...Please Wait");
            }
            Polynomial poly = new Polynomial(0, .5, .5);
            while (power > 1)
            {
                PolyInt(ref poly);
                power--;
            }
            Console.WriteLine("Formula: " + poly.ToString('x'));
            long[] points;
            if (string.IsNullOrEmpty(args["x"]))
            {
                string[] nums = args["x"].Split(' ');
                points = new long[nums.LongLength];
                for (long index = 0; index < points.LongLength; index++)
                {
                    points[index] = long.Parse(nums[index]);
                }
            }
            else
            {
                if ((string)args != null)
                {
                    string[] nums = ((string)args).Split(' ');
                    points = new long[nums.LongLength];
                    for (long index = 0; index < points.LongLength; index++)
                    {
                        points[index] = long.Parse(nums[index]);
                    }
                }
                else
                {
                    while (true)
                    {
                        Console.Write("What should the formula be evaluated at?");
                        string input = Console.ReadLine();
                        if (input.Length == 0)
                        {
                            return;
                        }
                        //else
                        try
                        {
                            Fraction eval = poly.Evaluate(long.Parse(input));
                            Console.WriteLine("Evaluation: " + eval.ToString() + " ≈ " + ((double)eval).ToString());
                        }
                        catch(FormatException)
                        {
                            Console.WriteLine("That is not a number.");
                        }
                    }
                }
            }
            foreach (long point in points)
            {
                Fraction eval = poly.Evaluate(point);
                if (pretty)
                {
                    Console.Write("Evaluation: " + eval.ToString() + " ≈ ");
                }
                Console.WriteLine(((double)eval).ToString());
            }
            if (pretty || inSuiGenerisConsole())
            {
                Console.Write("Press any key to continue...");
                Console.ReadKey(true);
                Console.WriteLine();
            }
        }
        static void PolyInt(ref Polynomial p)
        {
            p >>= 1;
            p.ConstTerm = 1;
            for (long index = 1; index <= p.LongDegree; index++)
            {
                p[index] /= index;
                p.ConstTerm -= p[index];
            }
        }
    }
}
