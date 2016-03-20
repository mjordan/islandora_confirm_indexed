# Islandora Confirm Indexed

## Overview

This utility module queries the Islandora Solr index when an object is ingested to confirm that it has been indexed in Solr. Developed as part of QA checks for a large migration, where FedoraGsearch was found to fail to index some objects on ingest. Only works with default auto-increment PIDS, not UUID PIDs.

When an object is ingested, this module queries Solr to see if the new object's PID is retrieved. If it isn't, the result of the query is logged. If Solr responds with a non-200 HTTP response code, the code and the PID are logged, as illustrated below:

```
[timestamp]    Not indexed    PID
[timestamp]    Error    HTTP response code (PID)
```

The module uses a "PID offset" to determine which PID it queries Solr for. In other words, it doesn't query Solr for the PID of the current object being intested, it queries Solr using the PID that is a few integers (the value of the offset) lower than the current object's PID. It needs to do this because the indexing operation for the current object is not yet complete when the module queries Solr. In practice, this means that at the end of a batch ingest, a number of objects will not have been queried and must be tested manually. This number is equal to the PID offset.

## Installation

* git clone git@git.lib.sfu.ca:mjordan/islandora_confirm_indexed.git
* drush -y en islandora_confirm_indexed

## Configuration

To change the default log file location from [Drupal install directory]/sites/default/files/islandora_confirm_indexed.txt, and to change other default settings, visit admin/islandora/tools/confirm_indexed.

## Usage

If this module is enabled, it will query Solr to confirm that a newly ingested object has been indexed, and if it does not find the object, it logs that result.

## License

GPL3

## Maintainer

* Mark Jordan
