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

 * Internationalization/i18n (latest dev Version!)
   (Only necessary for i18n_string translation)

Basic concepts
------------------

TODO

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
- Enable the following modules, this will also include all
  - Translation Management UI
  - Content translation Source UI
  - Microsoft Translator
- A translator has been automatically created. Go to the Translator management
  page at:

    Configuration > Regional and language > Translation Management > Translators

  Adjust the label to your liking and get an API key using the provided link in
  the settings. Then save the updated translator.

3) Translate

- Create a new piece of content of the multilingual content type defined before.
  Make sure to choose a language.
- Once the node has been saved, click on the "Translate" tab.
- Choose the language you want to translate the node to with the checkbox.
- Click on 'Request Translation' and the foreign language version of the node
  will be created immediately.
- Check the translated node!

For further options, see the documentation on drupal.org/documentation/modules/tmgmt.

== State of Module ==

This projects consists of many pluggable, independent elements. It is alpha
quality and not every plugin is in an equal state. The list below aims to give a
short overview of each translator and source plugin.

The management part itself is working well. It is possible to create and submit
jobs to various translators, get the translated text back into the system,
review and approve it. We're still working on the user interfaces, they are
unfinished and very basic.

==== Sources ====

- Content Translation
  Plugin and User Interface is working and tested. No User interface to
  translate multiple content items at once yet.

- Entity Translation
  The API is working, there is no user interface yet. If you know the entity
  translation module then we could use your help!

- Internationalization (I18n)
  Same story, the API works but we're missing a user interface.

=== Translators ===

- Microsoft Translator
  Machine translation, technically working well but the results are of course of
  varying quality.

- File translator
  Allows to export jobs into files and import them once they have been
  translated. Contains a pluggable system to support various file formats,
  currently XLIFF and HTML.

- Local Translator
  Allows to assign translation jobs to local users so that they can translate
  things in a unified interface with the ability to review it. Currently not
  working, needs to be adapted.

- myGengo
  Integrates with http://www.mygengo.com. Initial implementation done, ready for
  testing.

- Supertext
  Integrates with http://www.supertext.ch. Development has started, not working
  yet. Stay tuned!

- Nativy
  Integrates with http://www.nativy.com/. Initial implementation done but
  it is currently broken. The integration is also not satisfying yet.
