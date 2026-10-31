{
  description = "OpenMT2 Metin2 client";

  # nixpkgs stays pinned inside default.nix; the flake only locks the engine.
  inputs = {
    neoeter = {
      url = "github:open-mt2/neoeter";
      flake = false;
    };
    neoeter-python = {
      url = "github:open-mt2/neoeter-python";
      flake = false;
    };
  };

  outputs = { self, neoeter, neoeter-python }:
    let
      systems = [ "x86_64-linux" "aarch64-linux" "aarch64-darwin" ];
      forAllSystems = f: builtins.listToAttrs (map (system: {
        name = system;
        value = f system;
      }) systems);
    in
    {
      packages = forAllSystems (system:
        let
          client = import ./default.nix { inherit system neoeter neoeter-python; };
        in
        {
          metin2client = client;
          default = client;
        });
    };
}
