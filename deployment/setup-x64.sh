# Setup script for UNIX (x64 build)
# This will download and install proyekgen using GitHub releases
# with your suitable downloader (e.g curl, wget)
DOWNLOAD_URL="https://github.com/spirothdev/proyekgen/releases/latest"
DOWNLOAD_PROGRAM="none"
OUTPUT_DIR="/usr/bin"
OUTPUT_NAME="linux-x64-exec-setup.tar.xz"
OUTPUT_ARCHIVE_PROGRAM="tar"

locate_output_dir() {
	if [ "$EUID" -e 0 ]; then
		# User has root privileges, no need to modify output directory
		return
	fi

	echo "User does not have root privileges, installing proyekgen locally.."
	OUTPUT_DIR=$([ -d "$HOME/.local/bin" ] && "$HOME/.local/bin/proyekgen" || "$HOME/.proyekgen/bin")
}

locate_download_program() {
	if [ ! command -v "curl" >/dev/null 2>&1 ]; then
		DOWNLOAD_PROGRAM="curl"
	elif [ ! command -v "wget" >/dev/null 2>&1 ]; then
		DOWNLOAD_PROGRAM="wget"
	else; then
		echo "No suitable download program was found."
		exit 127
	fi
}

cleanup() {
	if [ -f "${DOWNLOAD_NAME}" ]; then
		rm $DOWNLOAD_NAME
	fi
}

download() {
	local DOWNLOAD_OPTIONS

	if [ "$DOWNLOAD_PROGRAM" -eq "curl" ]; then
		 DOWNLOAD_OPTIONS="-o ${OUTPUT_NAME}"
	elif [ "$DOWNLOAD_PROGRAM" -eq "wget" ]; then
		DOWNLOAD_OPTIONS="-O ${OUTPUT_NAME}"
	fi

	echo "Downloading ${OUTPUT_NAME} from ${DOWNLOAD_URL}..."
	$DOWNLOAD_PROGRAM $DOWNLOAD_URL $DOWNLOAD_OPTIONS

	if [ "$?" -neq 0 ]; then
		echo "There was an error occurred while downloading ${OUTPUT_NAME}."
		cleanup && exit 1
	fi
}

extract() {
	if [ command -v $OUTPUT_ARCHIVE_PROGRAM >/dev/null 2>&1 ]; then
		echo "Cannot extract ${OUTPUT_NAME} because tar is not found."
		cleanup && exit 127
	fi

	echo "Installing ${OUTPUT_NAME} to ${OUTPUT_DIR}..."
	$OUTPUT_ARCHIVE_PROGRAM -xvf $OUTPUT_NAME -C ${OUTPUT_DIR}
	cleanup
	echo "Done."
}

locate_output_dir
locate_download_program
download
extract