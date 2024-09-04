SELECT 
  list_aggregate(tau_pt, 'avg'),
  list_aggregate(tau_eta, 'avg'),
  list_aggregate(jet_pt, 'avg'),
  list_aggregate(jet_eta, 'avg'),
  list_aggregate(muon_pt, 'avg'),
  list_aggregate(muon_eta, 'avg'),
  list_aggregate(el_pt, 'avg'),
  list_aggregate(el_eta, 'avg'),
  list_aggregate(ph_pt, 'avg'),
  list_aggregate(ph_eta, 'avg')  
FROM singleMu;