(function (a) {

  function aggregateFactory (entity_id, entity_name, entity_type) {

    var box = $('<div class="record '+entity_type+'">'+entity_name+'</div>');

    var type = entity_type + 's';

    var link = a.util.loadEntityPic({type: type, id: entity_id, style: 'thumbnail', link: true});

    link.append(box);

    return link;

  }

  $(function () {

    var user_id = a.scope.user_id;

    var participating = $('div.user-view.aggregate.events');

    a.getUserParticipating({user_id: user_id, start: 0, end: 10}, function (data) {

      data = data || "{}";

      data = JSON.parse(data);

      if (data.fail) {

        participating.append('<p class="notice">This user is not participating in any events at this time</p>');

        return;

      }
       
      for (var i = 0; i < data.length; i++) {

        var event = data[i];
        var event_id = event.event_id;
        var name = event.name;

        var elem = aggregateFactory(event_id, name, "event");

        participating.append(elem);

      }

    });

    var isMember = $('div.user-view.aggregate.rsos');

    a.getUserMembership({user_id: user_id, start: 0, end: 10}, function (data) {

      data = data || "{}";

      data = JSON.parse(data);

      if (data.fail) {

        var my_account = a.scope.my_account;

        if (my_account) {

          isMember.append('<p>You are not a member of any RSO\'s. Feel free to <a href="' + a.siteRoot + 'rsos">Explore RSO\'s</a>');

        }
        else {

          isMember.append('<p class="notice">This user is not a member of any RSO\'s</p>');

        } 

      }
       
      for (var i = 0; i < data.length; i++) {

        var rso = data[i];
        var rso_id = rso.rso_id;
        var name = rso.name;

        var elem = aggregateFactory(rso_id, name, "rso");

        isMember.append(elem);

      }
       
    });

  });

})(app);
