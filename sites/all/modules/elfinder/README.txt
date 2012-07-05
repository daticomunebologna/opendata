elFinder file manager integration module for Drupal
===================================================


System Requirements:

 - Drupal 6.x or 7.x
 - elFinder 1.1+

Optional:
 
 - Wysiwyg module
 - Wysiwyg editor:
 
    - CKEditor
    - FCKeditor
    - TinyMCE 3
    - BUEdtor
    - jWYSIWYG (Drupal 6.x, jWYSIWYG 0.93+, jquery 1.3+, jquery ui 1.7+)
    - YUI Editor
    - WYMeditor

  
Installation:

  1. Unpack archive contents into modules/elfinder
  2. Get latest elFinder at https://sourceforge.net/projects/elfinder/ and extract 'elfinder-x.y' contents to sites/all/libraries/elfinder
  3. Enable elFinder module in Site Building > Modules (Modules menu in Drupal 7)
  4. Add 'use file manager' permission to users who will be able to use elFinder at User Management > Permissions (Modules > elFinder > Permissions in Drupal 7)
  5. Enable elFinder checkbox in your editor Wysywyg profile at Configuration > Wysiwyg profiles > <Editor Profile> Buttons and Plugins
  
Usage:

 1. Administration backend
  
  a. Open /elfinder url (or ?q=elfinder if seo-capable urls disabled)
  b. Go to Administer page. Click 'Files' under Content section.

 2. Inside CKEditor (FCKeditor)
  2.1. Open Image Properties dialog in editor
  2.2. Click Browse Server button near image url
 
 3. Inside TinyMCE
  3.1. Open Insert/edit image dialog
  3.2. Click Browse button near image url
  
 4. Inside BUEditor
  4.1. Click 'Insert/edit image'
  4.2. Click 'Browse'
  4.3. Select file
  4.4. Enter alt text then click OK
 
 5. Inside jWYSIWYG
  5.1. Make sure that you have jWYSYWIG 0.93+ (http://github.com/akzhan/jwysiwyg/), jQuery 1.3+ and jQuery UI 1.7+ installed
  5.2. Click 'Insert image' button
  5.3. Click 'Browse Server' button near image url
 
 6. Inside YUI Editor
  6.1. Click 'Insert Image' button
  6.2. Click 'Browse Server' button near image url

 7. Inside WYMeditor
  7.1. Click 'Image' button
  7.2. Click 'Browse Server' button near image url
 

  
Known Issues:

 - Not all editors supported
 - Not all manager features supported
 - Conflicting with some devel module features
 - Conflicting with IMCE module
 - Incompatible with admin_menu_toolbar module


