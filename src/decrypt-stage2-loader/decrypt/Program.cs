using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Security.Cryptography;
using System.Text;
using System.Threading.Tasks;

namespace decrypt
{
    class Program
    {
        static string smethod_36(string string_0)
        {
            string password = "fdfdfrtertert";
            string s = "fdfdfrtertert";
            string s2 = "@1B2c3D4sfg5F6g7H8";
            byte[] bytes = Encoding.ASCII.GetBytes(s2);
            byte[] bytes2 = Encoding.ASCII.GetBytes(s);
            byte[] array = Convert.FromBase64String(string_0);
            Rfc2898DeriveBytes rfc2898DeriveBytes = new Rfc2898DeriveBytes(password, bytes2, 2);
            byte[] bytes3 = rfc2898DeriveBytes.GetBytes(32);
            ICryptoTransform transform = new RijndaelManaged
            {
                Mode = CipherMode.CBC
            }.CreateDecryptor(bytes3, bytes);
            MemoryStream memoryStream = new MemoryStream(array);
            CryptoStream cryptoStream = new CryptoStream(memoryStream, transform, CryptoStreamMode.Read);
            byte[] array2 = new byte[checked(array.Length - 1 + 1)];
            int count = cryptoStream.Read(array2, 0, array2.Length);
            memoryStream.Close();
            cryptoStream.Close();
            return Encoding.UTF8.GetString(array2, 0, count);
        }
        static string smethod_30(string string_0)
        {
            string s = "美明八零会家美";
            RijndaelManaged rijndaelManaged = new RijndaelManaged();
            MD5CryptoServiceProvider md5CryptoServiceProvider = new MD5CryptoServiceProvider();
            string result;
            byte[] array = new byte[32];
            byte[] sourceArray = md5CryptoServiceProvider.ComputeHash(Encoding.ASCII.GetBytes(s));
            Array.Copy(sourceArray, 0, array, 0, 10);
            Array.Copy(sourceArray, 0, array, 15, 10);
            rijndaelManaged.Key = array;
            rijndaelManaged.Mode = CipherMode.ECB;
            ICryptoTransform cryptoTransform = rijndaelManaged.CreateDecryptor();
            byte[] array2 = Convert.FromBase64String(string_0);
            string @string = Encoding.ASCII.GetString(cryptoTransform.TransformFinalBlock(array2, 0, array2.Length));
            result = @string;
            return result;
        }
        static void Main(string[] args)
        {
            if (args[0] == "smethod_30")
            {
                Console.WriteLine(smethod_30(args[1])); 
            }
            if (args[0] == "smethod_36")
            {
                Console.WriteLine(smethod_36(args[1]));
            }
        }
    }
}
