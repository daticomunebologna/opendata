Translation Management Tools (tmgmt)
-------------------------------------

A collection of tools to facilitate the translation of text elements in Drupal.

Requirements
------------------

Translation Management Tool was built for Drupal 7. There will be no backport.

To use Translation Management Tool you need to install and activate the
following modules:

 * Entity API
 * Views
 * Chaos Tools (Required for Views)
 * Views Bulk Operations
 * Content Translation
 * Locale
 * Rules

Optional dependencies:
 * Internationalization/i18n (latest dev Version!)
   (Only necessary for i18n_string translation)
 * Entity Translation (only for entity sources)

Basic concepts
------------------

With TMGMT installed, the 'translate' tab of a node changes. You can choose
one or more languages to translate the node to and 'Request a translation' with
the corresponding button.

A translation job is created for each language chosen. It will run through the
following states:

Unprocessed     Translation requested in the 'translate' tab of a node.
                Settings of the job (label set, translator chosen) defined.
                The job was saved.
Active          The job is in the process of being translated. Depending on
                the chosen translator, the actual translation happens auto-
                matically or by a human being.
                In all cases the job is returned to the job queue for review.
                When the review is done, the status of the job item goes from
                'needs review' to 'accepted'.
Finished        The job has been accepted and the translated node was created

A job deleted from the list is not visible any more. You can choose to purge
jobs automatically from the list after a defined time span in the settings.

The project also provides overviews for the node and entity sources that allow
to translate multiple pieces of content in a single job and see the current
translation status for your site content.

Getting started
------------------

The first simple translation job using Microsoft's translation service.

1) Preparation

- Make sure you have downloaded all of the listed dependencies.
- Define a second language using locale
- Modify one content type to be multilingual. Choose 'Enabled, with translation'
  from the Publishing Options / Multilingual support.

2) Set up Translation Management Tools

- Download tmgmt module
- Download tmgmt_microsoft module
- Enable the following modules, this will also include all dependencies
  - Translation Management UI
  - Content translation Source UI
  - Microsoft Translator
- A translator has been automatically created. Go to the Translator management
  page at:

    Configuration > Regional and language > Translation Management > Translators

  Adjust the label to your liking and get a client ID and client secret using
  the provided link in the settings. Then save the updated translator.

- Adjust the Auto Acceptance settings to your liking. You can choose to accept
  jobs without review by checking  'Auto accept finished translations' for each
  of your translators individually.

3) Translate

- Create a new piece of content of the multilingual content type defined before.
  Make sure to choose a language.
- Once the node has been saved, click on the "Translate" tab.
- Choose the language you want to translate the node to with the checkbox.
- Click on 'Request Translation' and the foreign language version of the node
  will be created immediately.
- If the auto acceptance is not set, find the job in the jobs queue and choose
  the 'review' link. Accept the translation and the translated node is created.
- Check the translated node!

For further options, see the documentation on http://drupal.org/node/1445790.

== State of Module ==

This projects consists of many pluggable, independent elements. It is alpha
quality and not every plugin is in an equal state. The list below aims to give a
short overview of each translator and source plugin.

IMPORTANT: As of alpha2, the external translators have been moved to a separate
           project

The management part itself is working well. It is possible to create and submit
jobs to various translators, get the translated text back into the system,
review and approve it. The user interfaces are already quite usable and we
continue to work on further improvements.

==== Sources ====

- Content Translation
  Plugin and User Interface is working and tested. There is an interface for
  bulk translation which also provides a translation status for each piece of
  content and language.

- Entity Translation
  Plugin and User Interface is working and tested. There is an interface for
  bulk translation, the translation status is displayed as well. Note that the
  overview requires a current Drupal 7.x-dev snapshot or 7.15 once available.

- Internationalization (I18n)
  The API works but we're missing a user interface.

=== Translators ===

- Microsoft Translator
  Machine translation, technically working well but the results are of course of
  varying quality.
  Moved to http://drupal.org/project/tmgmt_microsoft.

- File translator
  Allows to export jobs into files and import them once they have been
  translated. Contains a pluggable system to support various file formats,
  currently XLIFF and HTML.

- Local Translator
  The current code has been removed from the code base because the plugin is
  currently unfortunately broken. It is being re-created as part of the
  Translation Management Server GSoC Project, you can follow the development in
  the following sandbox: http://drupal.org/sandbox/sumsi/1668042.

- Gengo (Previously named MyGengo)
  Integrates with http://www.gengo.com. Works, has tests, a few known issues
  that need to be resolved.
  Moved to http://drupal.org/project/tmgmt_mygengo.

- Supertext
  Integrates with http://www.supertext.ch. Has been updated and improved,
  including automated tests.
  Moved to http://drupal.org/project/tmgmt_supertext.

- Nativy
  Integrates with http://www.nativy.com/. Has been updated to use the new,
  improved API and is the first translator with support for a review process
  within TMGMT.
  Moved to http://drupal.org/project/tmgmt_nativy.

- Google
  New translator that allows to use the machine translation API from Google.
  Available at Moved to http://drupal.org/project/tmgmt_google.
