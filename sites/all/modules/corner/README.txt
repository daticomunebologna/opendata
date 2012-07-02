
-- SUMMARY --

This module lets you add configurable corners to your site.

I've tried to keep the functionality of the module simple, and strived to 
follow all the conventions and best practices for Drupal 7 module developement.

For a full description visit the project page:
http://drupal.org/project/corner

Bug reports, feature suggestions and latest developments:
http://drupal.org/project/issues/corner



-- REQUIREMENTS --

* Optional: http://drupal.org/project/pngfix
  Use the pngfix module to ensure that 24 bit PNG images are handled correctly in 
  IE 5.5 and IE 6.



-- INSTALLATION --

* Install as usual, see http://drupal.org/node/70151 for further information.



-- CONFIGURATION --

* Configure user permissions in 
  Administration >> People >> Permissions >> Corner module:

  - Administer corners: Controls who can add/configure/delete corners.

  - Use PHP for corner visibility: Controls who can use PHP for corner 
    visibiliy. Note: incorrect PHP-code can break your site and this permission 
    should only be given to persons who knows (at least a little) PHP.

* Customize module settings in 
  Administration >> Configuration >> User interface >> Corners

* See CUSTOMAZATION section below for information about adding more corner
  images.



-- CUSTOMIZATION --

* Additional corner images can be up uploaded to the corner/images
  directory. Make sure that the images are valid 24-bit PNG (PNG-24) images
  for best result when using transparency.

* You have two options to customize the apperance of the corners:

  1) Override the default style via CSS in your theme. See the default
     corner.tpl.php or corner.css files for the available style selectors.

  2) Copy the corner.tpl.php file to your theme and edit the output.


* Aspiring designers might find these two Photoshop tutorials useful for creating custom corners:
   
  1) The 'I LOVE Drupal' corner was made using this tutorial
     http://psdtuts.com/interface-tutorials/making-a-message-strip-in-photoshop/

  2) The pagepeel corner was made using this tutorial/template:
     http://vibr8bros.com/freebies/free-photoshop-template-page-peel-for-any-image



-- TODOS/FIXES --

* Fix and ensure tests are up-to-date and functional

* Review all SQL queries (should use the new database API)

* Add margin/padding at top of page according to the administration menu/tooldbar



-- PLANNED ENHANCEMENTS --

* Allow directory for storing images/conrners configurable (ref. http://drupal.org/node/539816)

* Update corner visibility settings to mimmic that of blocks in Drupal 7 (needed?)



-- CONTACT --

Author/current maintainers:
* Eirik L�hr (kirie) - http://drupal.org/user/93869

This project has been sponsored by:
* Ethos Technologies
  Visit http://www.ethos.com.cn for more information.
* Symbio - port to Drupal 7.x
  Visit http://www.symbio.com for more information
