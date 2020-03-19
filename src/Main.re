Random.init(int_of_float(Js.Date.now())); // seed

let getNewPosts = (): Js.Promise.t(list(Reddit.redditPost)) =>
  Fetch.fetch(Reddit.subredditURL)
  |> Js.Promise.then_(Fetch.Response.text)
  |> Js.Promise.then_(text =>
       switch (Reddit.parseRedditResponse(text)) {
       | json => Js.Promise.resolve(json.data.children)
       | exception e => Js.Promise.reject(e)
       }
     );

let getRandomRedditPost = () =>
  getNewPosts()
  |> Js.Promise.then_(posts =>
       List.nth(posts, Random.int(List.length(posts))) |> Js.Promise.resolve
     );

type redditPostResult = {
  url: string,
  title: string,
};

let rec getRandomImagePost = (): Js.Promise.t(ref(redditPostResult)) =>
  getRandomRedditPost()
  |> Js.Promise.then_((post: Reddit.redditPost) => {
       let data = ref({url: "", title: ""});
       if (!post.data.is_self && !post.data.media) {
         data := {url: post.data.url, title: post.data.title};
         Js.Promise.resolve(data);
       } else {
         getRandomImagePost();
       };
     });
