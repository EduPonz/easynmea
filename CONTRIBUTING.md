# Contribution Guidelines

The following documents constitutes a set of guidelines to which contributors are expected to adhere when contributing to *EasyNMEA*.

*EasyNMEA* is an open source, free-to-use, community driven project; and as such, contributions to expand or improve the code base or documentation are most welcome and encouraged; we just want the best product possible for our community.
There are several ways in which you can contribute to *EasyNMEA*:

* [Code of Conduct](#code-of-conduct)
* [What is a contribution](#what-is-a-contribution)
* [Opening an issue](#opening-an-issue)
* [Participate on open discussions](#participate-on-open-discussions)
* [Submitting a pull request](#submitting-a-pull-request)
* [Developer Certificate of Origin](#developer-certificate-of-origin)
* [Code Coverage](#code-coverage)

## Code of Conduct

This project and everyone participating in it is governed by the *EasyNMEA* [Code of Conduct](CODE_OF_CONDUCT.md).
By participating, you are expected to uphold this code at all times.
Please report unacceptable behavior to e.ponzs@gmail.com.

## What is a contribution

Basically, a contribution is everything you do for this project, from opening an issue, to submitting a pull request, promoting adoption on any platform, etc.
Before opening an issue or submitting a pull request, please browse the repository [issues](https://github.com/EduPonz/easynmea/issues) and [pull requests](https://github.com/EduPonz/easynmea/pulls), and maybe take a look at the [project boards](https://github.com/EduPonz/easynmea/projects); what you have encountered or want to implement may already already be answered or in process of development.

## Opening an issue

The easiest way for you to start contributing to the *EasyNMEA* project is to open an issue on this GitHub repository.
*EasyNMEA* is open for contributions of any kind; bug reports, feature requests, questions, etc.
When opening an issue, please take the following into account:

1. Try to use positive and welcoming language.
We sincerely appreciate your contribution and want to help each other.
1. Describe your needs or concerns in detail.
The more information provided, the better our understanding will be, and the more satisfied you will get in the end.
1. Be patient.
Bear in mind that your issue might not be answered or addressed right away.
If a couple of days go by without any feedback, please send a friendly ping by commenting on the issue.
1. Provide feedback.
Once your issue is addressed, please provide feedback so we can improve the process as we go.

## Participate on open discussions

You don't need to open an issue or implement some feature to contribute to the project.
Simply engaging in any of the open discussion and bring new opinions or points of view has enormous value for the project.

## Submitting a pull request

If writing an issue does not do it for you because you have and idea of what should be done and how, and you have the energy and time to directly contribute to the code base or documentation, submitting a pull request is your way to go.
Please mind that by doing so, you accept to abide to the pull request process followed on the *EasyNMEA* project, meaning:

1. First, open an issue ticket labeled as enhancement defining what you attempt to accomplish in terms of user experience or behaviour; or simply chose one of the open ones.
1. Open a pull request linked to your ticket with some design documentation that can kickoff a discussion.
   The design proposal should contain a description of all the tests that would need to be created to cover not just the main functionality, but corner and negative cases as well.
   This design step is most likely the most important one of the entire process, as there may be open discussions that lead to changes on the design or even the feature description itself; be open to those.
   Moreover, the design is a great contribution in itself, since it allows other people to work on the task with a clear view of the what, where, how, and why.
   In fact, feel free to make design proposals for open feature requests, even when they come from others, and even if you are not planning on implementing them.
   Furthermore, feel free to participate in any open design proposal.
1. The *EasyNMEA* project tries to follow a test driven development approach.
   This means that once a design proposal is approved by maintainers, the next step is to start writing the tests described in it.
   It'd be ideal if these tests could be reviewed before starting the implementation, as they will set the base line for knowing when the feature is ready to be incorporated into the main stream.
1. Of course, not everything can be covered during the design phase, and you'll need to take decisions and create new tests while implementing (it's just the natural way of things).
   Just try to clearly document your decisions along the way (including why you took a specific choice and not any of the alternatives); this will help reviewers to understand your train of thought.
1. As for branch names, we normally use `features/*` for expansions or refactors, `hotfix/*` for small fixes, and `bugfix/*` for larger fixes.
1. Test your changes locally before submission.
The Continuous Integration pipelines will still run the tests, but this way the process becomes smoother and the "Fixing errors" commits and re-runs are avoided.
If you need any help setting up, please let us know by opening an issue.
1. Be prepared to received criticism, and potentially tons of change requests.
Please do not take criticism as personal, we really appreciate your contributing effort.
1. Mind that [LICENSE.md](LICENSE.md) applies to all contributions made to this repository.

## Developer Certificate of Origin

Contributors must sign-off each commit by adding a `Signed-off-by: ...`
line to commit messages to certify that they have the right to submit
the code they are contributing to the project according to the
[Developer Certificate of Origin (DCO)](https://developercertificate.org/).

## Code Coverage

All contributions to the project must increase line code coverage.
