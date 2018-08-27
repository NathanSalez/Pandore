#!/bin/bash

# Ce script a pour but de supprimer les fichiers data.pass présents dans le répertoire pandoreRelease/files

rm -f ../pandoreRelease/files/data.pass
rm -f ../pandoreRelease/files/.help/.datapass

echo "Fichiers data.pass supprimés. L'application a été réinitialisée."
