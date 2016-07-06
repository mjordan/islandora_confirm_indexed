# Islandora Confirm Indexed

## Overview

This utility module queries the Islandora Solr index for a specific PID to confirm that the object the PID identifies has been indexed in Solr. Only works with default auto-increment PIDS, not UUID PIDs. The module was developed as part of the QA processes for a large migration where FedoraGsearch was found to fail to index some objects on during batch ingest.

When an object is ingested, this module queries Solr to see if an existing object's PID is retrieved. If it isn't, the result of the query is logged. If Solr responds with a non-200 HTTP response code, the code and the PID are logged, as illustrated below:

```
[timestamp]    Not indexed    PID
[timestamp]    Error    HTTP response code (PID)
```
There is a config option to also log successful queries, in which case the log entries will look like this:

```
[timestamp]    Indexed    PID
```

The module uses a "PID offset" to determine which PID it queries Solr for. In other words, it doesn't query Solr for the PID of the current object being ingested, it queries Solr using the PID that is a few integers (the value of the offset) lower than the PID of the object being ingested. It does this because the indexing operations for the current object are not yet complete when the module queries Solr. The ingestion of an object is used solely as the trigger to issue a query that confirms that an already existing object (whose PID is equal to the PID of the current object minus the PID offset) has been indexed.

In practice, using this offset means that at the end of a batch ingest, a number of objects will not have been queried to confirm that they have been indexed in Solr and must be tested manually. This number of objects will be equal to the PID offset and will have PIDs ranging from the PID of the last object ingested to the PID of the last object ingested minus the offset. However, during consecutive batch ingests using the same namespace, the set of objects that were not tested during the previous batch ingest will be tested during succeeding batch ingest, so manual confirmation that they have been indexed will not be necessary.

The module also provides a drush script for querying a list of PIDs to determine if they are indexed in Solr. Details on its usage are provided below.

## Installation

* git clone https://github.com/mjordan/islandora_confirm_indexed
* drush -y en islandora_confirm_indexed

## Configuration

To change the default log file location from [Drupal install directory]/sites/default/files/islandora_confirm_indexed.txt, and to change other default settings, visit admin/islandora/tools/confirm_indexed.

If you get false reports that an object is not indexed during batch ingests, you may want to increase the value of the PID offset, which will give Solr more time to complete the indexing of objects before being queried.

## Usage

This module is intended to be enabled during large batch ingests to detect whether Fgsearch fails to index objects. It should probably be disabled on production Islandora instances (although it will not cause any harm if it is not).

The drush command `drush islandora_confirm_indexed --pid_file=/tmp/pids.txt --unindexed_log=/tmp/unindexed.log` will read the contents of the file at `--pid_file` and query each PID in Solr, writing out the PIDs of objects that it can't find to the file specified at `--unindexed_log`.

The input PID file must contain one PID per line. The output log file will also contain one PID per line.

## License

GPL3

## Maintainer

Mark Jordan
