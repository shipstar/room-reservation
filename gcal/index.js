import google from 'googleapis'
import loadSecret from './authorize'
import moment from 'moment'
import _ from 'lodash'

import dataStore from '../dataStore'

function postEvents(duration, auth) {
  // Refer to the Node.js quickstart on how to setup the environment:
  // https://developers.google.com/google-apps/calendar/quickstart/node
  // Change the scope to 'https://www.googleapis.com/auth/calendar' and delete any
  // stored credentials.
  var calendar = google.calendar('v3');
  var conferenceRoomName = 'Test Room';
  const now = moment()
  var coreId = 'abc'

  var event = {
    'summary': 'Room In Use',
    'location': conferenceRoomName,
    'description': 'This room is in use.',
    'start': {
      'dateTime': now.format(),
    },
    'end': {
      'dateTime': now.clone().add(duration, 'minutes').format(),
    },
    'reminders': {
      'useDefault': false,
      'overrides': []
    },
  };

  calendar.events.insert({
    auth: auth,
    calendarId: 'primary',
    resource: event,
  }, function(err, eventRsp) {
    if (err) {
      console.log('There was an error contacting the Calendar service: ' + err);
      return;
    }
    console.log('Event created: %s', eventRsp.id);
    // console.log(eventRsp)
    dataStore[coreId].coreOnline = true
    dataStore[coreId].occupied = true
    dataStore[coreId].eventID = eventRsp.id
    dataStore[coreId].eventRsp = eventRsp
    console.log(dataStore[coreId])

    // move to updateEvents function
    var event2 = eventRsp
    event2.end.dateTime = now.format()

    return

    calendar.events.update({
      auth: auth,
      calendarId: 'primary',
      resource: event2,
      eventId: event2.id
    }, function(err, eventRsp) {
      if (err) {
        console.log('There was an error contacting the Calendar service: ' + err);
        return;
      }
    });

  });
}

function updateEvents(duration, auth) {
  // Refer to the Node.js quickstart on how to setup the environment:
  // https://developers.google.com/google-apps/calendar/quickstart/node
  // Change the scope to 'https://www.googleapis.com/auth/calendar' and delete any
  // stored credentials.
  var calendar = google.calendar('v3');
  var conferenceRoomName = 'Test Room';
  const now = moment()
  var event = {
    'summary': 'Room In Use',
    'location': conferenceRoomName,
    'description': 'This rooms is in use',
    'start': {
      'dateTime': now.format(),
    },
    'end': {
      'dateTime': now.add(duration, 'minutes').format(),
    },
    'reminders': {
      'useDefault': false,
      'overrides': []
    },
  };

  calendar.events.insert({
    auth: auth,
    calendarId: 'primary',
    resource: event,
  }, function(err, event) {
    if (err) {
      console.log('There was an error contacting the Calendar service: ' + err);
      return;
    }
    console.log('Event created: %s', event.htmlLink);
    console.log(Object.keys(event))
  });
}

function startEvent(googleCalendarId, { duration }) {
  console.log('starting event for ', googleCalendarId, duration)

  return new Promise((resolve, reject) => {
    const resp = loadSecret(_.partial(postEvents, duration))
    console.log(resp)
    // resolve(resp)
    // console.warn('resolving')
    // resolve('12345')
  })
}

function endEvent(googleCalendarId, { eventId }) {
  console.log('ending event for: ', googleCalendarId, eventId)

  loadSecret(postEvents)
}

export {
  startEvent,
  endEvent
}
