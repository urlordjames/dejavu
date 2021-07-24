let pkgs = import <unstable> {};
in pkgs.stdenv.mkDerivation rec {
	pname = "dejavu_server";
	version = "0.0.1";

	src = ./.;

	nativeBuildInputs = with pkgs; [
		cmake
	];

	buildInputs = with pkgs; [
		drogon
	];

	preConfigure = ''
		export cmakeFlags="$cmakeFlags -DSTATIC_ROOT=$out/extras/static"
	'';
}
