using Neoeter.Scripting;
using Neoeter.Scripting.Packs;

[assembly: ScriptEntryPoint(typeof(OpenMT2.Scripts.Program))]

namespace OpenMT2.Scripts;

// Minimal entry point for the .NET script runtime: the C# counterpart of
// system.py. Port the game scripts here module by module; every native module
// the Python scripts import (app, net, chr, wndMgr, ...) is available through
// Native.Import / Native.ImportDynamic.
public static class Program
{
    public static int Main(string[] args)
    {
        NativeModule host = Native.Import("scriptHost");
        bool debug = host.Constant<int>("DEBUG") != 0;
        Native.Log.Trace($"OpenMT2.Scripts started on {host.Constant<string>("NAME")} (debug={debug}, args=[{string.Join(", ", args)}])");

        // Packs can be redirected from C#: e.g. serve a modded file instead of
        // the original, or add a resolver that reads from a mod folder.
        if (Directory.Exists("mods"))
            Pack.AddResolver(new DirectoryPackResolver("mods"));

        dynamic app = Native.ImportDynamic("app");
        try
        {
            // Raises RuntimeError(CREATE_DEVICE, CREATE_WINDOW, ...) on failure.
            app.Create("OpenMT2", 1024, 768, 1);
        }
        catch (ScriptException exception)
        {
            Native.Log.Error($"app.Create failed: {exception.NativeMessage}");
            return 1;
        }

        app.Loop();
        return 0;
    }
}
