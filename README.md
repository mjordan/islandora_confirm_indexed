# Islandora Log Ingest

## Overview

This utility module logs the CONTENTdm source URL and the local Islandora URL for objects as they are ingested into Islandora. It writes a tab-(or other delimiter)-separated log file that contains the following columns:

```
[timestamp] source_url islandora_url
```

Its purpose it to generate a list of URL pairs that can be used in manual before/after QA tests. The source of the CONTENTdm URL is the MODS `<identifier>` element containing the source object's URL in CONTENTdm, e.g., `<identifier type="uri" invalid="yes" displayLabel="Migrated From">http://content.lib.sfu.ca/cdm/ref/collection/CT_1930-34/id/17583</identifier>` element, which should be present for every migrated object.

This module is a fork of https://github.com/mjordan/islandora_log_ingest, customized for SFU Library's migration from CONTENTdm.

## Installation

* git clone git@git.lib.sfu.ca:mjordan/lslandora_log_ingest.git
* drush -y en islandora_log_ingest

## Configuration

To change the default log file location from [Drupal install directory]/sites/default/files/islandora_log_ingest.txt, and to change other default settings, visit admin/islandora/tools/log_ingest.

## Usage

If this module is enabled, all objects ingested after the module is endabled will be logged.


## Maintainer

* Mark Jordan
