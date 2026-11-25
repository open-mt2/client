using Neoeter.Scripting;

[assembly: ScriptEntryPoint(typeof(ScriptRuntimeTests.Program))]

namespace ScriptRuntimeTests;

public static class Program
{
    private static bool s_nestedRan;

    public static int Main(string[] args)
    {
        // Re-entered without arguments by app.RunPythonFile below.
        if (args.Length == 0)
        {
            s_nestedRan = true;
            return 0;
        }

        int failures = 0;
        void Check(bool condition, string what)
        {
            if (!condition)
            {
                failures++;
                Native.Log.Error($"[FAIL] {what}");
            }
        }

        Check(args.SequenceEqual(["alpha", "beta"]), "entry arguments");

        NativeModule host = Native.Import("scriptHost");
        Check(host.Constant<string>("NAME") == "dotnet", "scriptHost.NAME");
        Check(host.Constant<string>("COMMAND_LINE") == "alpha beta", "scriptHost.COMMAND_LINE");
        Check(host.Constant<int>("DEBUG") is 0 or 1, "scriptHost.DEBUG");

        // Engine modules from the client and from the bindings library.
        foreach (string name in new[] { "app", "chr", "net", "player", "item", "pack", "wndMgr", "grp", "dbg", "snd" })
            Check(Native.TryImport(name) != null, $"module {name}");
        Check(Native.Import("app").HasFunction("Loop"), "app.Loop");

        // app.RunPythonFile goes through the active backend (Script_RunFile).
        Check(Native.Import("app").Call<int>("RunPythonFile", "scripts/OpenMT2.Scripts.dll") == 1 && s_nestedRan, "app.RunPythonFile -> .NET");

        return failures;
    }
}
