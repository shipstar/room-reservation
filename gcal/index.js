import google from 'googleapis'
import loadSecret from './authorize'
import moment from 'moment'
import _ from 'lodash'

function postEvents(duration, auth) {
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
      //   {'method': 'email', 'minutes': 24 * 60},
      //   {'method': 'popup', 'minutes': 10},
      // ],
    },
  };
  // console.log(event)

  // return

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
