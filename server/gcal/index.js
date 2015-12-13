import google from 'googleapis'
import loadSecret from './authorize'
import moment from 'moment'
import _ from 'lodash'

import dataStore from '../dataStore'

function postEvents(calendarId, duration, auth) {
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
    calendarId: calendarId,
    resource: event,
  }, function(err, eventRsp) {
    if (err) {
      console.log('There was an error contacting the Calendar service: ' + err);
      return;
    }
    console.log('Event created: %s', eventRsp.id);
    dataStore[coreId].coreOnline = true
    dataStore[coreId].occupied = true
    dataStore[coreId].eventId = eventRsp.id
    dataStore[coreId].eventRsp = eventRsp
  });
}

function stopEvents(calendarId, event, auth) {
  // Changes the end dateTime of a Gcal event to the current time.
  var calendar = google.calendar('v3');
  // var conferenceRoomName = 'Test Room';
  const now = moment()
  var coreId = 'abc'

  event.eventRsp.end.dateTime = now.format()
  event.eventRsp.description = 'Event is over.'

  calendar.events.update({
    auth: auth,
    calendarId: calendarId,
    resource: event.eventRsp,
    eventId: event.eventId
  }, function(err, eventRsp2) {
    if (err) {
      console.log('There was an error contacting the Calendar service: ' + err);
      return;
    }
    dataStore[coreId].coreOnline = true
    dataStore[coreId].occupied = false
    dataStore[coreId].eventId = null
    dataStore[coreId].eventRsp = null
  });
}

function startEvent(googleCalendarId, { duration }) {
  console.log('starting event for ', googleCalendarId, duration)

  return new Promise((resolve, reject) => {
    const resp = loadSecret(_.partial(postEvents, googleCalendarId, duration))
    // console.log(resp)
    // resolve(resp)
    // console.warn('resolving')
    // resolve('12345')
  })
}

function endEvent(googleCalendarId, { event }) {
  console.log('ending event for: ', googleCalendarId, event.eventId)

  return new Promise((resolve, reject) => {
    const resp = loadSecret(_.partial(stopEvents, googleCalendarId, event))
    // console.log(resp)
    // resolve(resp)
    // console.warn('resolving')
    // resolve('12345')
  })
}

export {
  startEvent,
  endEvent
}
