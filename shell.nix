let
  sources = import ./npins;
  pkgs = import sources.nixpkgs { };
in
pkgs.mkShell {
  name = "dvv";

  packages = [
    pkgs.minisign
  ];

  nativeBuildInputs = [ ];

  buildInputs = [ ];

  env = { };

  shellHook = ''
  '';
}
