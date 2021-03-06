#!/bin/bash

function pack_archive () {
  local PREFIX FILES TARFILE
  PREFIX=$1
  TARFILE=$2
  FILES=$(git ls-files)
  tar  \
    --transform="s+^+${PREFIX}+"  \
    -zcf  ${TARFILE} \
    ${FILES}
}

function list_contains_count () {
  local FIRST REST
  FIRST=$1
  shift
  REST=$*
  LIST_COUNT=0
  for X in $REST; do
    if [ "X$X" == "X${FIRST}" ]; then
      LIST_COUNT=$((LIST_COUNT + 1))
    fi
  done
}

set -x
ALL_ARGS=$*
NAME=souffleur
CHANGELOG=${NAME}.changes
HARMAT_TEMP_DIR=${NAME}.orig
VERSION=$(sh version.sh)
echo "version is " $VERSION

ARCHIVE=${NAME}-${VERSION}
HARMAT_ARCHIVE=${NAME}_${VERSION}
TARFILE=${HARMAT_ARCHIVE}.tar.gz
HARMAT_TARFILE=${TARFILE}
HARMAT_DSCFILE=${HARMAT_ARCHIVE}.dsc
HARMAT_PACKDIR=harmattan_packaging
HARMAT_DIFF=${HARMAT_ARCHIVE}.diff.gz


pack_archive ${ARCHIVE}/ ${TARFILE}
TEMPDIR=build_${NAME}_${VERSION}
mkdir -p $TEMPDIR
cp $TARFILE $TEMPDIR
cd $TEMPDIR

tar zxf $TARFILE

#mad -t harmattan-platform-api dpkg-source -b ${ARCHIVE}

dpkg-source -b ${ARCHIVE}

cd ..

#tar zcf debian.tar.gz debian/

RPM_PACKDIR=rpm_packaging
RPM_TARFILE=${HARMAT_TARFILE}

list_contains_count obs $ALL_ARGS
WANT_OBS=$LIST_COUNT
echo "WANT_OSC is $WANT_OBS"

DEB_FILES=" \
          ${TEMPDIR}/$HARMAT_TARFILE \
           ${TEMPDIR}/$HARMAT_DSCFILE \
          "
HAVE_ALL_DEB_FILES=yes
for F in $DEB_FILES ; do
  if [ ! -f $F ] ; then
    HAVE_ALL_DEB_FILES=no
  fi
done

if [ ! "X$HAVE_ALL_DEB_FILES" == "Xyes" ] ; then
  echo "Could not generate all of $DEB_FILES "
  echo "quitting"
  exit 1
fi

mkdir -p ${HARMAT_PACKDIR}
cp ${DEB_FILES} ${HARMAT_PACKDIR}
if [ 0$WANT_OBS -ge 1 ]; then
  set -x
  cp $DEB_FILES meego_obs/
  set +x
fi

echo ${VERSION} > ${RPM_PACKDIR}/pack-version
echo ${NAME} > ${RPM_PACKDIR}/pack-name

set -x
cp ${TEMPDIR}/${HARMAT_TARFILE} ${CHANGELOG} ${RPM_PACKDIR}
set +x


echo -n " remove temp dir "$TEMPDIR " [Y/n] ? "
read YORN
if [[ "X$YORN" == "X" ]]; then
  YORN="Y"
fi
if [[ "X$YORN" == "Xy" ]]; then
  YORN="Y"
fi
if [[ "X$YORN" == "XY" ]]; then
  rm -r $TEMPDIR
fi


