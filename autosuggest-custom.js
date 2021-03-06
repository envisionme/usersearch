if (Drupal.jsEnabled) {
  $(document).ready(function() {
    $("#edit-search-string").autoSuggest("/usersearch/autosuggest.json", {
          minChars: 2,
          selectedItemProp: "name",
          searchObjProps: "name",
          emptyText: '',
          resultClick: function(data){
            window.location = "/user/" + data.attributes.value;},
          selectionAdded: function(elem){
            elem.hide();},
          formatList: function(data, elem){
            var my_image = data.image;
            var new_elem = elem.html(data.image).append('<div class="as-name">' + data.name + "</div>");
            return new_elem;
          },
          queryParam: 'as',
    });
  });
}
