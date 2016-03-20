# Islandora Confirm Indexed

## Overview

This utility module queries the Islandora Solr index when an object is ingested to confirm that it has been indexed in Solr. Developed as part of QA checks for a large migration, where FedoraGsearch was found to fail to index some objects on during batch ingest. Only works with default auto-increment PIDS, not UUID PIDs.

When an object is ingested, this module queries Solr to see if an existing object's PID is retrieved. If it isn't, the result of the query is logged. If Solr responds with a non-200 HTTP response code, the code and the PID are logged, as illustrated below:

```
[timestamp]    Not indexed    PID
[timestamp]    Error    HTTP response code (PID)
```
There is a config option to also log successful queries, in which case the log entries will look like this:

```
[timestamp]    Indexed    PID
```

The module uses a "PID offset" to determine which PID it queries Solr for. In other words, it doesn't query Solr for the PID of the current object being ingested, it queries Solr using the PID that is a few integers (the value of the offset) lower than the PID of the object being ingested. It needs to do this because the indexing operation for the current object is not yet complete when the module queries Solr. The ingestion of an object is used solely as the trigger to issue a query that confirms that an already existing object (with a PID n - offset) has been indexed.

In practice, using this offset means that at the end of a batch ingest, a number of objects will not have been queried to confirm that they have been indexed in Solr and must be tested manually. This number of objects will be equal to the PID offset and will have PIDs ranging from the PID of the last object ingested to the PID of the last object ingested minus the offset. However, during consecutive batch ingests using the same namespace, the set of objects that were not tested during the previous batch ingest will be tested during succeeding batch ingest, so manual confirmation that they have been indexed will not be necessary.

## Installation

* git clone git@git.lib.sfu.ca:mjordan/islandora_confirm_indexed.git
* drush -y en islandora_confirm_indexed

## Configuration

To change the default log file location from [Drupal install directory]/sites/default/files/islandora_confirm_indexed.txt, and to change other default settings, visit admin/islandora/tools/confirm_indexed.

If you get false reports that an object is not indexed during batch ingests, you may want to increase the value of the PID offset, which will give Solr more time to complete the indexing of objects before being queried.

## Usage

This module is intended to be enabled and use during large batch ingests to detect whether Fgsearch fails to index objects. It should probably be disabled on production Islandora instances (although will not cause any harm if it is not).

## License

GPL3

## Maintainer

Mark Jordan
